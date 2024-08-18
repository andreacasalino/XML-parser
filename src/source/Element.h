/**
 * Author:    Andrea Casalino
 * Created:   04.09.2024
 *
 * report any bug to andrecasa91@gmail.com.
 **/

#pragma once

#include <XML-Parser/Tag.h>

#include <optional>
#include <string_view>
#include <unordered_map>
#include <variant>

namespace xmlPrs::parse {
struct Element {
  std::string_view remaining;
};

struct Tag : Element {
  TagPtr make();

  std::string_view name;
};

struct Preamble : Element {
  static std::optional<Preamble> parse(std::string_view buffer);

  std::unordered_map<std::string_view, std::string_view> metaData;
};

struct TagOpen : Tag {
  static std::optional<TagOpen> parse(std::string_view buffer);

  TagPtr make();

  std::unordered_multimap<std::string_view, std::string_view> attributes;
  bool withClosure;
};

struct TagClose : Tag {
  static std::optional<TagClose> parse(std::string_view buffer);
};

struct Comment : Element {
  static std::optional<Comment> parse(std::string_view buffer);
};

struct Null : Element {};

namespace detail {
template <typename... Alternatives> struct ParseWithAlternatives {
  ParseWithAlternatives(std::string_view buffer) {
    if (!parse_<Alternatives...>(buffer)) {
      result = Null{};
    }
  }

  std::variant<Null, Alternatives...> result;

private:
  template <typename First, typename Second, typename... Others>
  bool parse_(std::string_view buffer) {
    return parse_<First>(buffer) || parse_<Second, Others...>(buffer);
  }

  template <typename Last> bool parse_(std::string_view buffer) {
    auto maybe_result = Last::parse(buffer);
    if (maybe_result.has_value()) {
      result.template emplace<Last>(std::move(maybe_result.value()));
      return true;
    }
    return false;
  }
};
} // namespace detail

template <typename... Alternatives>
std::variant<Null, Alternatives...> parse_element(std::string_view buffer) {
  return detail::ParseWithAlternatives<Alternatives...>{buffer}.result;
}

} // namespace xmlPrs::parse
