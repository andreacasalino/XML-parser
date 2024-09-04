/**
 * Author:    Andrea Casalino
 * Created:   04.09.2024
 *
 * report any bug to andrecasa91@gmail.com.
 **/

#pragma once

#include <XML-Parser/Error.h>

#include <filesystem>
#include <memory>
#include <ostream>
#include <string>
#include <string_view>
#include <unordered_map>
#include <vector>
#include <sstream>

namespace xmlPrs {
class Parser;

class Tag;
using TagPtr = std::unique_ptr<Tag>;

using Attributes = std::unordered_multimap<std::string, std::string>;
using Tags = std::unordered_multimap<std::string_view, TagPtr>;

/** @brief The object storing the data of an xml like structure
 */
class Tag {
public:
  virtual ~Tag() = default;

  Tag(std::string name);

  /** @brief The copied tag is assumed to not have any father
   */
  Tag(const Tag &o);
  Tag &operator=(const Tag &o);

  Tag(Tag &&o) noexcept = default;
  Tag &operator=(Tag &&o) = default;

  const auto &getName() const { return name; }

  const auto &getContent() const { return content; }
  void setContent(std::string c) { content = std::move(c); }

  /** @return false in case the tag is a root
   */
  bool hasFather() const { return father; }

  /** @return the name of tag containing this one.
   * @throw if the tag has no father
   */
  const Tag *getFather() const { return father; }
  Tag *getFather() { return father; }

  /** @return a reference to the attributes contained in this tag.
   */
  const Attributes &getAttributes() const { return this->attributes; };
  Attributes &getAttributes() { return this->attributes; };

  const Tags &getNested() const { return nested; };

  /** @brief Get a tag at a specified position, starting from this tag.
   * The term position refers to a chain of tag names. Essentially, the path is
   * the series of tag to traverse to get a certain nested tag.
   * Consider this example:
   * <A><B><C><D></D></C></B></A>.
   * The path of tag D from A is: B/C/D/. At the same time, the path of tag D
   * from B is: C/D/. Therefore, If tou wanna get D you should pass as
   * input B/C/D/. Instead, when you want to get
   * D from the Tag wrapping B, you should pass as input C/D/.
   * @param[in] the path to traverse to get the tag of interest from this one
   * @return the desired tag
   * @throw if the tag in the passed location does not exist
   */
  const Tag &getDescendant(const std::filesystem::path &path) const {
    return getDescendant_(path);
  }
  Tag &getDescendant(const std::filesystem::path &path) {
    const Tag &res = getDescendant_(path);
    return const_cast<Tag &>(res);
  }

  /** @brief remove this tag from the parent.
   * @throw if this tag has no parent.
   */
  void remove() {
    if (nullptr == this->father) {
      throw Error("can't remove root tag");
    }
    auto it = findInFather();
    this->father->nested.erase(it);
  }

  /** @brief rename this tag
   */
  void rename(const std::string &name);

  Tag &addNested(TagPtr toAdd) {
    std::string_view name{toAdd->name.data()};
    toAdd->father = this;
    return *nested.emplace(name, std::move(toAdd))->second.get();
  }

  /** @brief adds a nested tag with the passed name and returns
   * a reference to the newly created tag.
   * @param[in] the name of the tag to add
   * @return the newly created tag.
   */
  Tag &emplaceNested(const std::string &tag_name) {
    auto clone = std::make_unique<Tag>(tag_name);
    clone->father = this;
    return *nested.emplace(clone->name.data(), std::move(clone))->second;
  }

  /** @brief returns the first nested tag with the passed name.
   * If no tag with that name exists, creates and returns and empty one
   */
  Tag &operator[](const std::string &tag_name);

  void print(std::ostream &stream_to_use) const;

  bool same(const Tag &o) const;

private:
  Tags::iterator findInFather();

  const Tag &getDescendant_(const std::filesystem::path &path) const;

  std::size_t depth() const {
    std::size_t ancestors = 0;
    for (const Tag *cursor = this; cursor->father != nullptr;
         cursor = cursor->father, ++ancestors) {
    }
    return ancestors;
  }

  // data
  Tag *father = nullptr;
  std::string name;
  Attributes attributes;
  Tags nested;
  std::string content;
};

std::ostream &operator<<(std::ostream &stream, const Tag &tag);

bool operator==(const Tag &a, const Tag &b);

std::string toString(const Tag& subject);

} // namespace xmlPrs
