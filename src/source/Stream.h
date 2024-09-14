/**
 * Author:    Andrea Casalino
 * Created:   04.09.2024
 *
 * report any bug to andrecasa91@gmail.com.
 **/

#pragma once

#include <istream>
#include <string_view>
#include <variant>

namespace xmlPrs::parse {
struct Next {
  std::string_view before_tag;
  std::string_view tag;
};

class StringStream {
  StringStream(const std::string &source) : remaining_{source.data()} {}

  bool eos() const { return remaining_.empty(); }

  Next next();

private:
  std::string_view remaining_;
};

class IStream {
  IStream(std::istream &stream) : stream_{stream} {}

  bool eos() const { return stream_.eof(); }

  Next next();

private:
  char nextInStream_() {
    char res;
    stream_.read(&res, 1);
    return res;
  }

  std::istream &stream_;
  std::string before_tag;
  std::string tag;
};

using Stream = std::variant<StringStream, IStream>;

} // namespace xmlPrs::parse
