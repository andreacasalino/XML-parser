/**
 * Author:    Andrea Casalino
 * Created:   04.09.2024
 *
 * report any bug to andrecasa91@gmail.com.
 **/

#pragma once

#include <memory>
#include <optional>
#include <string>
#include <string_view>
#include <vector>

namespace xmlPrs {

inline void shift(std::string_view &recipient, std::size_t len) {
  recipient = std::string_view{recipient.data() + len, recipient.size() - len};
}

template <char Symbol> bool match(char v);

template <char First, char Second, char... Others> bool match(char v) {
  return match<First>(v) || match<Second, Others...>(v);
}

template <char Symbol> bool match(char v) { return v == Symbol; }

template <bool LeftRight, char... ToSKip>
inline void strip(std::string_view &recipient) {
  while (!recipient.empty()) {
    if constexpr (LeftRight) {
      if (!match<ToSKip...>(recipient.front())) {
        break;
      }
      shift(recipient, 1);
    } else {
      if (!match<ToSKip...>(recipient.back())) {
        break;
      }
      recipient = std::string_view{recipient.data(), recipient.size() - 1};
    }
  }
}

template <char... ToSKip> inline void stripL(std::string_view &recipient) {
  strip<true, ToSKip...>(recipient);
}

template <char... ToSKip> inline void stripR(std::string_view &recipient) {
  strip<false, ToSKip...>(recipient);
}

template <char... Delimiters>
std::string_view cut(std::string_view &recipient) {
  std::size_t len = 0;
  std::string_view remaining = recipient;
  while (!remaining.empty() && !match<Delimiters...>(remaining.front())) {
    shift(remaining, 1);
    ++len;
  }
  std::string_view res = std::string_view{recipient.data(), len};
  recipient = remaining;
  return res;
}

template <typename Last>
std::pair<bool, std::string_view> doesFrontMatch(std::string_view recipient,
                                                 Last last);

template <typename First, typename Second, typename... Others>
// <is matching, remaining after the match>
std::pair<bool, std::string_view> doesFrontMatch(std::string_view recipient,
                                                 First first, Second second,
                                                 Others &&...others) {
  auto maybe_res = doesFrontMatch(recipient, first);
  if (!maybe_res.first) {
    return maybe_res;
  }
  recipient = maybe_res.second;
  return doesFrontMatch(recipient, second, std::forward<Others>(others)...);
}

template <typename Last>
std::pair<bool, std::string_view> doesFrontMatch(std::string_view recipient,
                                                 Last last) {
  stripL<' ', '\n', '\t'>(recipient);
  std::pair<bool, std::string_view> res;
  res.first = false;
  if constexpr (std::is_same_v<Last, char>) {
    if (recipient.front() == last) {
      res.first = true;
      shift(recipient, 1);
      res.second = recipient;
    }
  }
  if constexpr (std::is_same_v<Last, const char *>) {
    std::string_view wrapper{last};
    if (wrapper.size() <= recipient.size() &&
        std::string_view{recipient.data(), wrapper.size()} == wrapper) {
      res.first = true;
      shift(recipient, wrapper.size());
      res.second = recipient;
    }
  } else {
    static_assert("Unsopported");
  }
  return res;
}

struct ViewsCollection {
  ViewsCollection() = default;

  ViewsCollection &operator+=(std::string_view val) {
    if (!val.empty()) {
      values.emplace_back(val);
    }
    return *this;
  }

  // returns empty string if content is made only by /n and spaces
  std::string toString() const;

private:
  std::vector<std::string_view> values;
};

} // namespace xmlPrs
