#include "T_structure.h"
#include <unordered_set>

Root generate_T_structure() {
  Root result;
  auto &tag_B = result.addNested("B");
  tag_B.getAttributes().emplace("B1", "b1");
  result.addNested("C");
  auto &tag_E = result.addNested("A").addNested("D").addNested("E");
  tag_E.getAttributes().emplace("E1", "e1");
  auto &tag_A = tag_E.getFather().getFather();
  tag_A.getAttributes().emplace("A1", "a1");
  tag_A.getAttributes().emplace("A2", "a2");
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
  auto &tag_B = *root.getNested().find("B")->second;
  Attributes attributes_B = Attributes{{"B1", "b1"}};
  if (attributes_B != tag_B.getAttributes()) {
    return false;
  }

  auto &tag_A = *root.getNested().find("A")->second;
  Attributes attributes_A = Attributes{{"A2", "a2"}, {"A1", "a1"}};
  if (attributes_A != tag_A.getAttributes()) {
    return false;
  }
  if (!has_children_list(tag_A, {"D"})) {
    return false;
  }

  auto &tag_D = *tag_A.getNested().find("D")->second;
  if (!has_children_list(tag_D, {"E"})) {
    return false;
  }

  auto &tag_E = *tag_D.getNested().find("E")->second;
  Attributes attributes_E = Attributes{{"E1", "e1"}};
  if (attributes_E != tag_E.getAttributes()) {
    return false;
  }

  return true;
}
