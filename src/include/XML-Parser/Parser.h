/**
 * Author:    Andrea Casalino
 * Created:   04.09.2024
 *
 * report any bug to andrecasa91@gmail.com.
 **/

#pragma once

#include <XML-Parser/Tag.h>

#include <filesystem>
#include <ostream>
#include <unordered_map>

namespace xmlPrs {
using MetaData = std::unordered_map<std::string, std::string>;

struct ParseResult {
  MetaData metaData;
  TagPtr root;
};

ParseResult parseString(const std::string &content);

ParseResult parseStream(std::istream &stream);

ParseResult parseFile(const std::filesystem::path &path);

} // namespace xmlPrs
