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
#include <algorithm>

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

void normalize_content(std::string& content) {
    remove_symbol(content, '\n');
    remove_symbol(content, '\t');
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
      if (' ' != content[s]) {
          return true;
      }
  }
  return false;
}

using TagsRaw = std::vector<std::vector<std::string>>;

bool is_comment(const std::vector<std::string>& tag_content) {
    if (tag_content.empty()) {
        return false;
    }
    const auto& front = tag_content.front();
    if (front.size() < 3) {
        return false;
    }
    const auto& back = tag_content.back();
    if (back.size() < 2) {
        return false;
    }
    return ("!--" == std::string{ front, 0, 3 }) && ("--" == std::string{ back, 0, 2 });
}

void remove_comments(TagsRaw& subject) {
    TagsRaw result;
    result.reserve(subject.size());
    for (auto& element : subject) {
        if (!is_comment(element)) {
            result.emplace_back(std::move(element));
        }
    }
    subject = std::move(result);
}

TagsRaw slice_tags(const std::string &fileContent) {
  TagsRaw tags;
  auto openTagPositions = find_symbol(fileContent, '<');
  auto closeTagPositions = find_symbol(fileContent, '>');
  if (openTagPositions.empty()) {
    throw Error("no tags found");
  }
  if (openTagPositions.size() != closeTagPositions.size()) {
    throw Error("number of opened tag don't match the closed ones");
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
      throw Error("found an empty tag");
    }
    ++itOpen;
    if ((itOpen != openTagPositions.end()) && (*itOpen > (*itClose + 1)) &&
        are_not_spaces(fileContent, *itClose + 1, *itOpen)) {
      throw Error("found bad syntax for the xml");
    }
  }
  remove_comments(tags);
  for (const auto& tag : tags) {
      if (tag.empty()) {
          throw Error("found empty tag");
      }
  }
  return tags;
}

using Field = std::pair<std::string, std::string>;
Field parse_field(const std::string &word) {
  std::vector<std::size_t> posEqual = find_symbol(word, '=');
  if (posEqual.size() != 1) {
    throw make_error("Invalid attribute: ", word, " has no =");
  }
  Field field = std::make_pair(std::string(word, 0, posEqual.front()),
                               std::string(word, posEqual.front() + 1));
  if ((field.second.front() != '\"') || (field.second.back() != '\"')) {
    throw make_error("Invalid attribute: ", word,
                     " attribute value not delimited by \"");
  }
  if (field.second.size() < 3) {
    throw make_error("Invalid attribute: ", word);
  }
  field.second = std::string(field.second, 1, field.second.size() - 2);
  return field;
}

struct TagAndName {
  std::string name;
  std::unique_ptr<Tag> tag;
};

struct TagsRawAndCursor {
  const TagsRaw &tags;
  std::size_t cursor;
};

TagAndName parse_tag(TagsRawAndCursor& to_parse) {
  const auto& opening = to_parse.tags[to_parse.cursor];
  std::unique_ptr<Tag> tag = std::make_unique<Tag>();
  // parse attributes
  auto it_fields = opening.begin();
  ++it_fields;
  std::for_each(it_fields, opening.end(), [&tag](const std::string& element) {
      auto field = parse_field(element);
      tag->getAttributes().emplace(std::move(field.first),
          std::move(field.second));
  });
  ++to_parse.cursor;
  // parse nested
  while (to_parse.cursor < to_parse.tags.size()) {
      const auto& front_current = to_parse.tags[to_parse.cursor].front();
      if (front_current == '/' + opening.front()) {
          if (to_parse.tags[to_parse.cursor].size() > 1) {
              throw make_error(front_current,
                  ": attributes cant't be nested to closing tags");
          }
          return TagAndName{ opening.front(), std::move(tag) };
      }
      else {
          auto tag_nested = parse_tag(to_parse);
          tag->addNested(tag_nested.name) = std::move(*tag_nested.tag);
      }
  }
  throw make_error("tag closing ", opening.front(), " not found");
}

Root parse_xml(TagsRawAndCursor& to_parse) {
  auto parsedRoot = parse_tag(to_parse);
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

std::variant<Root, Error> parse_xml_from_string(const std::string& content) {
    std::variant<Root, Error> result;
    try {
        std::string content_normalized = content;
        normalize_content(content_normalized);
        auto tags = slice_tags(content_normalized);
        // try parse first tag as preamble
        TagsRawAndCursor to_parse = TagsRawAndCursor{tags, 0};
        auto preamble_attributes = parse_preamble(tags.front());
        if (std::nullopt != preamble_attributes) {
            ++to_parse.cursor;
        }
        auto parsed = parse_xml(to_parse);
        result = Root(parsed.getName());
        Root& result_ref = std::get<Root>(result);
        if (std::nullopt != preamble_attributes) {
            result_ref.getPreambleAttributes() = std::move(*preamble_attributes);
        }
        Root& parsed_ref = parsed;
        static_cast<Tag&>(result_ref) = std::move(parsed_ref);
    }
    catch (const Error & e) {
        result = e;
    }
    catch (const std::exception & e) {
        result = Error{ e.what() };
    }
    return result;
}

std::variant<Root, Error> parse_xml(std::istream& stream) {
    return parse_xml_from_string(std::string{ std::istreambuf_iterator<char>(stream),
                                              std::istreambuf_iterator<char>() });
}

std::variant<Root, Error> parse_xml(const std::string &fileName) {
    std::ifstream stream(fileName);
    if (!stream.is_open()) {
        stream.close();
        return make_error(fileName, ": file not found");
    }
    return parse_xml(stream);
}

} // namespace xmlPrs
