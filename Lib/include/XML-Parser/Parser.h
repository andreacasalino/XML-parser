/**
 * Author:    Andrea Casalino
 * Created:   03.12.2019
 *
 * report any bug to andrecasa91@gmail.com.
 **/

#pragma once

#include <XML-Parser/Tag.h>

namespace xmlPrs {
/** @brief Slice a string into many pieces.
 * Example: "hello to   everybody" become {"hello", "to", "everybody"}.
 */
std::vector<std::string> slice_fragments(const std::string &toSplit);

/** @brief Parse the content of a textual file.
 * @throw in case the content of the file to parse is invalid or the file
 * location is not readable.
 */
Root parse_xml(const std::string &fileName);
} // namespace xmlPrs
