#include "T_structure.h"
#include <unordered_set>

Root generate_T_structure() {
  Root result;
  result.addNested("B");
  result.addNested("C");
  result.addNested("A").addNested("D").addNested("E");
  return result;
}

namespace {
bool has_children_list(const Tag &subject,
                       const std::unordered_set<std::string> &names) {
  const auto &nested = subject.getNested();
  if (nested.size() != names.size()) {
    return false;
  }
  for (const auto &name : names) {
    if (nested.find(name) == nested.end()) {
      return false;
    }
  }
  return true;
}
} // namespace

bool is_valid_T_structure(const Root &root) {
  if (!has_children_list(root, {"A", "B", "C"})) {
    return false;
  }
  auto &tag_A = *root.getNested().find("A")->second;
  if (!has_children_list(tag_A, {"D"})) {
    return false;
  }
  auto &tag_D = *tag_A.getNested().find("D")->second;
  if (!has_children_list(tag_D, {"E"})) {
    return false;
  }
  return true;
}
