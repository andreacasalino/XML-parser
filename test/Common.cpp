#include "Common.h"

#include <unordered_set>

namespace xmlPrs::test {

void TmpFile::clean() {
  if (std::filesystem::exists(path)) {
    std::filesystem::remove_all(path);
  }
}

TmpFile::TmpFile(const std::string &filename)
    : path{std::filesystem::temp_directory_path() / filename} {
  clean();
  std::filesystem::create_directories(path);
}

TmpFile::~TmpFile() { clean(); }

namespace {
  Tag make_T_structure() {
    Tag result{"Root"};
    auto &tag_B = result.emplaceNested("B");
    tag_B.getAttributes().emplace("B1", "b1");
    result.emplaceNested("C");
    auto &tag_E = result.emplaceNested("A").emplaceNested("D").emplaceNested("E");
    tag_E.getAttributes().emplace("E1", "e1");
    auto &tag_A = *tag_E.getFather()->getFather();
    tag_A.getAttributes().emplace("A1", "a1");
    tag_A.getAttributes().emplace("A2", "a2");
    return result;
  }
}

const Tag &get_T_structure() {
  static Tag result = make_T_structure();
  return result;
}

} // namespace xmlPrs::test
