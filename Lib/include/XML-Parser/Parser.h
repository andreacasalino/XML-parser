/**
 * Author:    Andrea Casalino
 * Created:   03.12.2019
 *
 * report any bug to andrecasa91@gmail.com.
 **/

#pragma once

#include <XML-Parser/Error.h>
#include <XML-Parser/Tag.h>
#include <variant>

namespace xmlPrs {
/** @brief Slice a string into many pieces.
 * Example: "hello to   everybody" becomes {"hello", "to", "everybody"}.
 */
std::vector<std::string> slice_fragments(const std::string &toSplit);

/** @brief Parse the content of a textual file and returns it
 * in case everything went well, otherwise the returned variant
 * is an expection explaining what went wrong.
 */
std::variant<Root, Error> parse_xml(const std::string &fileName);
} // namespace xmlPrs
