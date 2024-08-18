#pragma once

#include <XML-Parser/Tag.h>

#include <filesystem>
#include <map>
#include <set>
#include <string>

namespace xmlPrs::test {

class TmpFile {
public:
  TmpFile(const std::string &filename);
  ~TmpFile();

  const auto &get() const { return path; }

private:
  void clean();

  std::filesystem::path path;
};

///       root
///        /|\
///       / | \
///      /  |  \
///     B   A   C
///         |
///         D
///         |
///         E
///
/// tag A has attributes {"A1 = a1", "A2 = a2"}
/// tag B has attributes {"B1 = b1"}
/// tag E has attributes {"E1 = e1"}
const Tag &get_T_structure();

template <typename ToFind, typename ToPut>
std::string replace(const std::string &subject, const ToFind &toFind,
                    const ToPut &toPut) {
  std::string_view buffer{subject.data()};
  std::string res;
  while (!buffer.empty()) {
    auto next = buffer.find(toFind);

    if (next == std::string::npos) {
      res += buffer;
      break;
    }

    res += std::string_view{buffer.data(), next};
    res += toPut;

    std::size_t lenToSkip = next;
    if constexpr (std::is_same_v<char, ToFind>) {
      ++lenToSkip;
    } else {
      lenToSkip += toFind.size();
    }
    buffer =
        std::string_view{buffer.data() + lenToSkip, buffer.size() - lenToSkip};
  }
  return res;
}

} // namespace xmlPrs::test
