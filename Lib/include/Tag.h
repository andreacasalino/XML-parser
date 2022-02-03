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
class Parser;

class Tag;
using TagPtr = std::unique_ptr<Tag>;

/** @brief The object storing the data of an xml like structure
 */
class Tag {
public:
  /** @brief A new root is created, i.e. tag with no parent
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
   * @throw if the tag is a root and has not father
   */
  const Tag &getFather() const;
  Tag &getFather();

  /** @return a reference to the attributes contained in this tag.
   */
  inline const std::unordered_multimap<std::string, std::string> &
  getAttributes() const {
    return this->fields;
  };
  inline std::unordered_multimap<std::string, std::string> &getAttributes() {
    return this->fields;
  };

  /** @return an iterator capable of scrolling all the tags nested in this one
   */
  std::unordered_multimap<std::string, TagPtr> &getNested() { return nested; };
  const std::unordered_multimap<std::string, TagPtr> &getNested() const {
    return nested;
  };

  /** @brief Get a tag in a specified position, starting from this tag.
   * The term position refers to a chain of tag names. Essentially, the path is
   * the series of tag to traverse to get a certain nested tag.
   * Consider this example:
   * <A><B><C><D></D></C></B></A>.
   * The path of tag D from A is: {B,C,D}. At the same time, the path of tag D
   * from B is: {C,D}. Therefore, If tou wanna get D using
   * Tag_readable::Get_Nested(const std::list<std::string>& position) on Tag
   * wrapping A, you should pass as input {B,C,D}. Instead, when you want to get
   * D from a Tag_readable wrapping B, you should pass as input {C,D}.
   * @param[in] the sequence of tag to traverse to get the one of interest from
   * the one wrapped
   * @return the desired tag
   * @throw if the tag in the passed location does not exist
   */
  const Tag &getDescendant(const std::vector<std::string> &position) const;
  Tag &getDescendant(const std::vector<std::string> &position);

  /** @brief remove this tag from the parent.
   * @throw if this tag is a root.
   */
  void remove();

  /** @brief rename this tag if it is nested in a father tag
   *  @throw if this tag has no father
   */
  void rename(const std::string &name);

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
  Tag &addNested(const std::string &tag_name);

  void print(std::ostream &stream_to_use, const std::string &space_to_skip,
             const std::string &name) const;

private:
  std::unordered_multimap<std::string, TagPtr>::iterator findInFather();

  // data
  Tag *father = nullptr;
  std::unordered_multimap<std::string, std::string> attributes;
  std::unordered_multimap<std::string, TagPtr> nested;
};

class Root : public Tag {
public:
  explicit Root(const std::string &name);

  /** @return the name of this tag
   */
  inline const std::string &getName() const { return this->name; };

  /** @brief set the name of this tag.
   */
  void setName(const std::string &new_name) { name = new_name; };

  void print(std::ostream &stream_to_use) const;

private:
  std::string name;
};

std::ostream &operator<<(std::ostream &s, const Root &t);
} // namespace xmlPrs
