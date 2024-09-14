#include <Stream.h>
#include <Utils.h>

namespace xmlPrs::parse {

namespace {
std::size_t parse_next_tag(std::string_view remaninig) {
  std::size_t len = 0;
  while (!remaninig.empty()) {
    auto cut_out = cut<'>', '\"'>(remaninig);
    len += cut_out.size();
    if (remaninig.front() == '>') {
      ++len;
      break;
    }

    shift(remaninig, 1);
    ++len;
    auto quote_close_pos = remaninig.find('\"');
    if (quote_close_pos == std::string::npos) {
      len += remaninig.size();
      break;
    }
    shift(remaninig, quote_close_pos + 1);
    len += quote_close_pos + 1;
  }
  return len;
}
} // namespace

Next StringStream::next() {
  Next res;
  auto tag_open_pos = remaining_.find('<');
  if (tag_open_pos == std::string::npos) {
    res.before_tag = std::string_view{remaining_.data()};
    remaining_ = std::string_view{};
    return res;
  }
  remaining_ = std::string_view{remaining_.data(), tag_open_pos};
  std::size_t tag_len = parse_next_tag(remaining_);
  res.tag = std::string_view{remaining_.data(), tag_len};
  shift(remaining_, tag_len);
  return res;
}

Next IStream::next() {
  if (stream_.eof()) {
    return Next{};
  }

  char next;

  before_tag.clear();
  tag.clear();
  for (next = nextInStream_(); !stream_.eof() && next != '<';
       next = nextInStream_()) {
    before_tag += next;
  }
  if (stream_.eof()) {
    return Next{std::string_view{before_tag.data(), before_tag.size()},
                std::string_view{}};
  }

  tag += '<';
  bool inside_quotes = false;
  while (!stream_.eof()) {
    next = nextInStream_();
    tag += next;

    if (inside_quotes) {
      if (next == '\"') {
        inside_quotes = false;
      }
      continue;
    }

    if (next == '>') {
      break;
    }
  }
  return Next{std::string_view{before_tag.data(), before_tag.size()},
              std::string_view{tag.data(), tag.size()}};
}

} // namespace xmlPrs::parse
