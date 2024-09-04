/**
 * Author:    Andrea Casalino
 * Created:   04.09.2024
 *
 * report any bug to andrecasa91@gmail.com.
 **/

#pragma once

#include <stdexcept>
#include <sstream>
#include <string>
#include <string_view>

namespace xmlPrs {

namespace detail {

struct Formatter {
  template<typename ... ARGS>
  Formatter(const char* bfr, const ARGS& ... args)
  : remaining{bfr} {
    this->next<ARGS...>(args...);
    buffer << remaining;
  }

  std::string extract() const { return buffer.str(); }

private:
  template<typename First,typename Second, typename ... ARGS>
  bool next(const First& first, const Second& second, const ARGS& ... args) {
    return next<First>(first) &&
           next<Second, ARGS...>(second, args...);
  }
  
  template<typename Last>
  bool next(const Last& last) {
    auto next = remaining.find("{}");
    if(next == std::string::npos) {
      return false;
    }
    buffer << std::string_view{remaining.data(), next};
    buffer << last;
    remaining = std::string_view{remaining.data() + next + 2, remaining.size() - next - 2};
    return true;
  }

  std::stringstream buffer;
  std::string_view remaining;
};

}

template<typename First, typename ... ARGS>
std::string format(const char* buffer, const First& first, const ARGS& ... args) {
  return detail::Formatter{buffer, first, args...}.extract();
}

/** @brief A runtime error that can be raised when using any object in this
 * namespace
 */
class Error : public std::runtime_error {
public:
  template<typename First, typename ... ARGS>
  Error(const char* frmt, const First& first, const ARGS& ... args)
  : Error{format(frmt, first, args...)} {
  }

  explicit Error(const std::string &what);
};
} // namespace xmlPrs
