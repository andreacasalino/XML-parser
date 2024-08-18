/**
 * Author:    Andrea Casalino
 * Created:   04.09.2024
 *
 * report any bug to andrecasa91@gmail.com.
 **/

#include <Element.h>
#include <Utils.h>

#include <optional>

namespace xmlPrs::parse {
namespace {
struct Attribute : Element {
  std::string_view name;
  std::string_view value;
};

std::optional<Attribute> parse_attribute(std::string_view buffer) {
  auto name = cut<' ', '\n', '\t', '='>(buffer);
  if (name.empty()) {
    return std::nullopt;
  }

  if (auto maybe_result = doesFrontMatch(buffer, '=', '"');
      maybe_result.first) {
    buffer = maybe_result.second;
  } else {
    return std::nullopt;
  }

  auto end_pos = buffer.find('"');
  if (end_pos == std::string::npos) {
    return std::nullopt;
  }
  std::string_view val{buffer.data(), end_pos};
  shift(buffer, end_pos + 1);

  Attribute res;
  res.remaining = buffer;
  res.name = name;
  res.value = val;
  return res;
}

template <char... Delimiters> struct AttributesParser {
  template <typename Pred>
  static bool parse_attributes(std::string_view &buffer, Pred pred) {
    while (!buffer.empty()) {
      stripL<' ', '\n', '\t'>(buffer);

      if (match<Delimiters...>(buffer.front())) {
        break;
      }

      auto next = parse_attribute(buffer);
      if (next.has_value()) {
        pred(next.value());
        buffer = next->remaining;
      } else {
        return false;
      }
    }
    return true;
  }
};

} // namespace

std::optional<TagOpen> TagOpen::parse(std::string_view buffer) {
  if (buffer.front() != '<') {
    return std::nullopt;
  }
  shift(buffer, 1);

  stripL<' ', '\n', '\t'>(buffer);
  auto name = cut<' ', '\n', '\t', '/', '>'>(buffer);
  if (name.empty() || name.front() == '/') {
    return std::nullopt;
  }

  TagOpen res;
  res.name = name;
  if (!AttributesParser<'>', '/'>::parse_attributes(
          buffer, [&res](const Attribute &attribute) {
            res.attributes.emplace(attribute.name, attribute.value);
          })) {
    return std::nullopt;
  }

  if (buffer.front() == '>') {
    shift(buffer, 1);
    res.withClosure = false;
  } else if (auto maybe_result = doesFrontMatch(buffer, '/', '>');
             maybe_result.first) {
    buffer = maybe_result.second;
    res.withClosure = true;
  } else {
    return std::nullopt;
  }

  res.remaining = buffer;
  return res;
}

std::optional<Preamble> Preamble::parse(std::string_view buffer) {
  if (buffer.front() != '<') {
    return std::nullopt;
  }
  shift(buffer, 1);

  if (auto maybe_result = doesFrontMatch(buffer, '?', "xml");
      maybe_result.first) {
    buffer = maybe_result.second;
  } else {
    return std::nullopt;
  }

  Preamble res;
  if (!AttributesParser<'?'>::parse_attributes(
          buffer, [&res](const Attribute &attribute) {
            res.metaData[attribute.name] = attribute.value;
          })) {
    return std::nullopt;
  }

  if (auto maybe_result = doesFrontMatch(buffer, '?', '>');
      maybe_result.first) {
    buffer = maybe_result.second;
  } else {
    return std::nullopt;
  }

  res.remaining = buffer;
  return res;
}

namespace {
std::string to_str(const std::string_view &str) {
  return std::string{str.data(), str.size()};
}
} // namespace

TagPtr Tag::make() { return std::make_unique<xmlPrs::Tag>(to_str(name)); }

TagPtr TagOpen::make() {
  auto res = this->Tag::make();
  auto &res_attributes = res->getAttributes();
  for (const auto &[k, v] : attributes) {
    res_attributes.emplace(to_str(k), to_str(v));
  }
  return res;
}

std::optional<TagClose> TagClose::parse(std::string_view buffer) {
  if (buffer.front() != '<') {
    return std::nullopt;
  }
  shift(buffer, 1);

  if (auto maybe_result = doesFrontMatch(buffer, '/'); maybe_result.first) {
    buffer = maybe_result.second;
  } else {
    return std::nullopt;
  }

  stripL<' ', '\n', '\t'>(buffer);
  auto name = cut<' ', '\n', '\t', '>'>(buffer);
  if (name.empty()) {
    return std::nullopt;
  }

  if (auto maybe_result = doesFrontMatch(buffer, '>'); maybe_result.first) {
    buffer = maybe_result.second;
  } else {
    return std::nullopt;
  }

  TagClose res;
  res.remaining = buffer;
  res.name = name;
  return res;
}

std::optional<Comment> Comment::parse(std::string_view buffer) {
  static const std::string_view preamble = "<!--";
  static const std::string_view closure = "-->";

  if (auto maybe_result = doesFrontMatch(buffer, preamble.data());
      maybe_result.first) {
    buffer = maybe_result.second;
  } else {
    return std::nullopt;
  }

  auto end_pos = buffer.find(closure);
  if (end_pos == std::string::npos) {
    return std::nullopt;
  }

  shift(buffer, end_pos + closure.size());
  return Comment{buffer};
}

} // namespace xmlPrs::parse
