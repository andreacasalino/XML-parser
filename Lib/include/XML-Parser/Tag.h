/**
 * Author:    Andrea Casalino
 * Created:   03.12.2019
 *
 * report any bug to andrecasa91@gmail.com.
 **/

#pragma once

#include <memory>
#include <ostream>
#include <string>
#include <unordered_map>
#include <vector>

namespace xmlPrs {
class Name : public std::string {
  /// this class exists in order to avoid generating empty string name
public:
  template <typename... Args> Name(Args... args) : Name(std::string{args...}){};

  Name(std::string value);
};
} // namespace xmlPrs

namespace std {
template <> struct hash<xmlPrs::Name> {
  std::size_t operator()(const xmlPrs::Name &object) const {
    return std::hash<std::string>{}(object);
  }
};
} // namespace std

namespace xmlPrs {
class Parser;

class Tag;
class TagPtr : protected std::unique_ptr<Tag> {
  /// this class exists in order to not expose std::unique_ptr<Tag>::reset
public:
  TagPtr(Tag &&o);

  using std::unique_ptr<Tag>::get;
  using std::unique_ptr<Tag>::operator->;
  using std::unique_ptr<Tag>::operator*;
};

using Attributes = std::unordered_multimap<Name, std::string>;
using Tags = std::unordered_multimap<Name, TagPtr>;

/** @brief The object storing the data of an xml like structure
 */
class Tag {
public:
  /** @brief A new unnamed root is created, i.e. tag with no parent
   */
  Tag() = default;

  /** @brief The copied tag is assumed to not have any father
   */
  explicit Tag(const Tag &o);
  Tag &operator=(const Tag &o);

  explicit Tag(Tag &&o);
  Tag &operator=(Tag &&o);

  /** @return false in case the tag is a root
   */
  bool hasFather() const;
  /** @return the name of tag containing this one.
   * @throw if the tag has no father
   */
  const Tag &getFather() const;
  Tag &getFather();

  /** @return a reference to the attributes contained in this tag.
   */
  const Attributes &getAttributes() const { return this->attributes; };
  Attributes &getAttributes() { return this->attributes; };

  /** @return a reference to the first tag found with the passed name, or
   *  generates a tag with that name if none exists and returns it.
   *  @param[in] the name of the tag to access
   */
  Tag &operator[](const Name &tag_name);

  Tags &getNested() { return nested; };
  const Tags &getNested() const { return nested; };

  /** @brief Get a tag at a specified position, starting from this tag.
   * The term position refers to a chain of tag names. Essentially, the path is
   * the series of tag to traverse to get a certain nested tag.
   * Consider this example:
   * <A><B><C><D></D></C></B></A>.
   * The path of tag D from A is: {B,C,D}. At the same time, the path of tag D
   * from B is: {C,D}. Therefore, If tou wanna get D you should pass as
   * input {B,C,D}. Instead, when you want to get
   * D from a Tag_readable wrapping B, you should pass as input {C,D}.
   * @param[in] the sequence of tag to traverse to get the one of interest from
   * the one wrapped
   * @return the desired tag
   * @throw if the tag in the passed location does not exist
   */
  const Tag &getDescendant(const std::vector<Name> &position) const;
  Tag &getDescendant(const std::vector<Name> &position);

  /** @brief remove this tag from the parent.
   * @throw if this tag has no parent.
   */
  void remove();

  /** @brief rename this tag if it is nested in a father tag
   *  @throw if this tag has no father
   */
  void rename(const Name &name);

  /** @brief removes all the tags and the attributes
   */
  void clear();

  /** @brief removes all the tags nested by this object.
   */
  void clearNested();

  /** @brief adds a nested tag with the passed name and returns
   * a reference to the newly created tag.
   * @param[in] the name of the tag to add
   * @return the newly created tag.
   */
  Tag &addNested(const Name &tag_name);

  void print(std::ostream &stream_to_use, const std::string &space_to_skip,
             const std::string &name) const;

private:
  Tags::iterator findInFather();

  // data
  Tag *father = nullptr;
  Attributes attributes;
  Tags nested;
};

class Root : public Tag {
public:
  explicit Root(const Name &name = "Root");

  Attributes &getPreambleAttributes() { return preamble_attributes; }
  const Attributes &getPreambleAttributes() const {
    return preamble_attributes;
  }

  /** @return the name of the root
   */
  const Name &getName() const { return this->name; };

  /** @brief set the name of this root
   */
  void setName(const Name &new_name) { name = new_name; };

  void print(std::ostream &stream_to_use) const;

private:
  Name name;
  Attributes preamble_attributes;
};

std::ostream &operator<<(std::ostream &s, const Root &t);
} // namespace xmlPrs
