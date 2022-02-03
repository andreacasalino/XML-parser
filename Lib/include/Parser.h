/**
 * Author:    Andrea Casalino
 * Created:   03.12.2019
 *
 * report any bug to andrecasa91@gmail.com.
 **/

#pragma once

#include <Tag.h>
#include <list>

namespace xmlPrs {
/** @brief Interface for handling reading and writing of xml files.
 */
class Parser {
public:
  /** @brief Slice a string into many pieces.
   * Example: "hello to   everybody" become {"hello", "to", "everybody"}.
   */
  static std::list<std::string> sliceFragments(const std::string &toSplit);

  /** @brief Slices the content of a file into many TagContent data.
   * @throw in case the content of the file is invalid or the file location
   * is not readable.
   */
  typedef std::list<std::string> TagContent;
  static std::list<TagContent> sliceTags(const std::string &fileName);

  /** @brief Import the xml from the specified path.
   * In case the file is invalid, the same structure obtained using the default
   * c'tor is created.
   *  @param[in] the location of the file to read and parse
   */
  explicit Parser(const std::string &source_file);
  /** @brief Creates in interal structure by copying the passed tag.
   *  @param[in] the tag to copy
   */
  explicit Parser(const Tag &root);
  /** @brief Creates an empty structure having a single root tag, named "Root".
   */
  Parser();

  explicit Parser(const Parser &o);
  Parser &operator=(const Parser &o);
  explicit Parser(Parser &&o);
  Parser &operator=(Parser &&o);

  /** @return the root of the structure.
   */
  inline const Tag &getRootConst() const { return *this->root; };
  inline Tag &getRoot() { return *this->root; };

  /** @brief Prints the current structure in the specified location.
   * In case the file already exists, it is overwritten.
   *  @param[in] the destination file
   */
  void reprint(const std::string &file_name) const;

  friend std::ostream &operator<<(std::ostream &, const Parser &);

private:
  static Tag::TagPtr
  parse(std::list<Parser::TagContent>::const_iterator current,
        std::list<Parser::TagContent>::const_iterator end);
  // data
  Tag::TagPtr root;
};

std::ostream &operator<<(std::ostream &s, const Parser &p);
} // namespace xmlPrs
