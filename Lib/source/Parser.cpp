/**
 * Author:    Andrea Casalino
 * Created:   03.12.2019
 *
 * report any bug to andrecasa91@gmail.com.
 **/

#include <XML-Parser/Error.h>
#include <XML-Parser/Parser.h>
#include <fstream>
#include <optional>
#include <sstream>

namespace xmlPrs {
namespace {
template <typename T>
void cumulate(std::stringstream &stream, const T &element) {
  stream << element;
}

template <typename T, typename... Args>
void cumulate(std::stringstream &stream, const T &element, Args... args) {
  cumulate(stream, element);
  cumulate(stream, args...);
}

template <typename T1, typename T2, typename... Args>
Error make_error(const T1 &element_1, const T2 &element_2, Args... args) {
  std::stringstream stream;
  cumulate(stream, element_1, element_2, args...);
  return Error{stream.str()};
}
} // namespace

std::vector<std::string> slice_fragments(const std::string &toSplit) {
  std::istringstream iss(toSplit);
  std::vector<std::string> slices;
  while (!iss.eof()) {
    slices.emplace_back();
    iss >> slices.back();
    if (slices.back().empty())
      slices.pop_back();
  }
  return slices;
}

namespace {
void remove_symbol(std::string &subject, const char symbol) {
  auto it = subject.begin();
  while (it != subject.end()) {
    if (*it == symbol) {
      it = subject.erase(it);
    } else {
      ++it;
    }
  }
};

std::string read_content_from_file(const std::string &fileName) {
  std::ifstream t(fileName);
  if (!t.is_open()) {
    t.close();
    throw Error("impossible to read the file");
  }
  std::string content((std::istreambuf_iterator<char>(t)),
                      std::istreambuf_iterator<char>());
  remove_symbol(content, '\n');
  remove_symbol(content, '\t');
  return content;
}

std::vector<std::size_t> find_symbol(const std::string &content,
                                     const char &symbol) {
  std::vector<std::size_t> pos;
  size_t s = 0;
  for (const auto &c : content) {
    if (content[s] == symbol) {
      pos.emplace_back(s);
    }
    ++s;
  }
  return pos;
}

bool are_not_spaces(const std::string &content, const std::size_t &startPos,
                    const std::size_t &endPos) {
  for (std::size_t s = startPos; s < endPos; ++s) {
    if (' ' != content[s])
      return true;
  }
  return false;
}

using TagsRaw = std::vector<std::vector<std::string>>;
TagsRaw slice_tags(const std::string &fileContent) {
  TagsRaw tags;
  auto openTagPositions = find_symbol(fileContent, '<');
  auto closeTagPositions = find_symbol(fileContent, '>');
  if (openTagPositions.empty()) {
    throw Error("no open tag symbols were found");
  }
  if (openTagPositions.size() != closeTagPositions.size()) {
    throw Error("number of open tag symbols don't match the close ones");
  }
  auto itOpen = openTagPositions.begin();
  auto itClose = closeTagPositions.begin();
  if ((*itOpen != 0) && are_not_spaces(fileContent, 0, *itOpen)) {
    throw Error("found bad syntax for the xml");
  }
  if ((closeTagPositions.back() != (fileContent.size() - 1)) &&
      are_not_spaces(fileContent, closeTagPositions.back() + 1,
                     fileContent.size())) {
    throw Error("found bad syntax for the xml");
  }
  for (itClose; itClose != closeTagPositions.end(); ++itClose) {
    if (!((*itOpen + 1) < *itClose)) {
      throw Error("found bad syntax for the xml");
    }
    tags.emplace_back(slice_fragments(
        std::string(fileContent, *itOpen + 1, *itClose - *itOpen - 1)));
    if (tags.back().empty()) {
      throw Error("found empty tag");
    }
    ++itOpen;
    if ((itOpen != openTagPositions.end()) && (*itOpen > (*itClose + 1)) &&
        are_not_spaces(fileContent, *itClose + 1, *itOpen)) {
      throw Error("found bad syntax for the xml");
    }
  }
  return tags;
}

using Field = std::pair<std::string, std::string>;
Field parse_field(const std::string &word) {
  std::vector<std::size_t> posEqual = find_symbol(word, '=');
  if (posEqual.size() != 1) {
    throw Error("found invalid field");
  }
  Field field = std::make_pair(std::string(word, 0, posEqual.front()),
                               std::string(word, posEqual.front() + 1));
  if ((field.second.front() != '\"') || (field.second.back() != '\"')) {
    throw Error("found invalid field");
  }
  if (field.second.size() < 3) {
    throw Error("found invalid field");
  }
  field.second = std::string(field.second, 1, field.second.size() - 2);
  return field;
}

struct TagAndName {
  std::string name;
  std::unique_ptr<Tag> tag;
};

struct TagsRawDilimiters {
  const TagsRaw &tags;
  std::size_t start;
  std::size_t end;
};

TagAndName parse_tag(const TagsRawDilimiters &delimiters) {
  const auto &front = delimiters.tags[delimiters.start];
  const auto &back = delimiters.tags[delimiters.end];
  if (front.empty()) {
    throw Error("found empty tag");
  }
  if (back.size() > 1) {
    throw Error("found attributes nested to closing tag");
  }
  if (back.front() != '/' + front.front()) {
    throw make_error("tag closing ", front.front(), " not found");
  }
  auto it_fields = front.begin();
  const auto &tag_name = *it_fields;
  ++it_fields;
  std::unique_ptr<Tag> tag = std::make_unique<Tag>();
  // parse attributes
  for (it_fields; it_fields != front.end(); ++it_fields) {
    auto field = parse_field(*it_fields);
    tag->getAttributes().emplace(std::move(field.first),
                                 std::move(field.second));
  }
  // parse nested
  std::size_t cursor = delimiters.start + 1, cursor_end;
  while (cursor < delimiters.end) {
    std::string closingName = "/" + delimiters.tags[cursor].front();
    // find terminating tag
    cursor_end = cursor;
    for (cursor_end; cursor_end < delimiters.end; ++cursor_end) {
      if (delimiters.tags[cursor_end].front() == closingName) {
        break;
      }
    }
    auto nested_structure =
        parse_tag(TagsRawDilimiters{delimiters.tags, cursor, cursor_end});
    tag->addNested(nested_structure.name) = std::move(*nested_structure.tag);
    cursor = cursor_end + 1;
  }
  return TagAndName{tag_name, std::move(tag)};
}

Root parse_xml(const TagsRawDilimiters &delimiters) {
  if (delimiters.tags.size() < 2) {
    throw Error{"invalid file"};
  }
  auto parsedRoot = parse_tag(delimiters);
  Root result(parsedRoot.name);
  static_cast<Tag &>(result) = std::move(*parsedRoot.tag);
  return result;
}

std::optional<Attributes>
parse_preamble(const std::vector<std::string> &slices) {
  if (slices.empty()) {
    return std::nullopt;
  }
  if (slices.front() != "?xml") {
    return std::nullopt;
  }
  if (slices.back().back() != '?') {
    return std::nullopt;
  }
  std::optional<Attributes> result;
  result.emplace();
  if (1 == slices.size()) {
    return result;
  }
  auto begin = slices.begin();
  ++begin;
  std::vector<std::string> slices2(begin, slices.end());
  // remove terminating '?'
  slices2.back().pop_back();
  for (const auto &attr : slices2) {
    auto field = parse_field(attr);
    result->emplace(std::move(field.first), std::move(field.second));
  }
  return result;
}
} // namespace

std::variant<Root, Error> parse_xml(const std::string &fileName) {
  std::variant<Root, Error> result;
  try {
    auto raw_content = read_content_from_file(fileName);
    auto tags = slice_tags(raw_content);
    // try parse first tag as preamble
    std::size_t start_pos = 0;
    auto preamble_attributes = parse_preamble(tags.front());
    if (std::nullopt != preamble_attributes) {
      start_pos = 1;
    }
    auto parsed =
        parse_xml(TagsRawDilimiters{tags, start_pos, tags.size() - 1});
    result = Root(parsed.getName());
    Root &result_ref = std::get<Root>(result);
    if (std::nullopt != preamble_attributes) {
      result_ref.getPreambleAttributes() = std::move(*preamble_attributes);
    }
    Root &parsed_ref = parsed;
    static_cast<Tag &>(result_ref) = std::move(parsed_ref);
  } catch (const Error &e) {
    result = e;
  } catch (const std::exception &e) {
    result = Error{e.what()};
  }
  return result;
}
} // namespace xmlPrs
