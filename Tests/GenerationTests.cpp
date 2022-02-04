#include <catch2/catch_test_macros.hpp>
#include <catch2/generators/catch_generators.hpp>

#include <unordered_set>

#include <XML-Parser/Tag.h>
using namespace xmlPrs;

// TEST_CASE("Empty names", "[generation]") {
//   Tag tag;
//   tag.addNested();
// }

bool contains_expected_tag_names(
    const Tag &subject, const std::unordered_set<std::string> &tag_names) {
  for (const auto &name : tag_names) {
    auto nested_it = subject.getNested().find(name);
    if ((nested_it == subject.getNested().end()) ||
        (&nested_it->second->getFather() != &subject)) {
      return false;
    }
  }
  return true;
}

TEST_CASE("Xml construction and inspection", "[generation]") {
  Root structure;

  SECTION("root inspection") { CHECK_FALSE(structure.hasFather()); }

  SECTION("attributes generation") {
    Attributes attributes;
    for (const auto &[name, val] : attributes) {
      structure.getAttributes().emplace();
    }
    CHECK(attributes == structure.getAttributes());
  }

  SECTION("sub tags generation") {
    std::unordered_set<std::string> tag_names =
        std::unordered_set<std::string>{"tag1", "tag2", "element", "ref"};
    for (const auto &name : tag_names) {
      structure.addNested(name);
    }
    CHECK(contains_expected_tag_names(structure, tag_names));
  }

  SECTION("access descendant") {
    const std::size_t depth = GENERATE(2, 5, 10);
    const std::string dummy_attr_name = "dummy";
    const std::string dummy_attr_value = "value";

    Tag *cursor = &structure;
    for (std::size_t d = 0; d < depth; ++d) {
      cursor->addNested("B");
      cursor = &cursor->addNested("A");
    }
    cursor->getAttributes().emplace(dummy_attr_name, dummy_attr_value);

    std::vector<Name> path(depth, "A");
    auto &tag_ref = structure.getDescendant(path);
    auto dummy_attr = tag_ref.getAttributes().begin();
    CHECK(dummy_attr->first == dummy_attr_name);
    CHECK(dummy_attr->second == dummy_attr_value);
  }
}

TEST_CASE("Tag copy", "[generation]") {}

TEST_CASE("Tag move", "[generation]") {}

TEST_CASE("Xml modification", "[generation]") {
  Root structure;

  SECTION("rename tag") {
    auto &tagB_ref = structure.addNested("B");
    auto &tagA_ref = structure.addNested("A");

    tagA_ref.rename("C");
    std::unordered_set<std::string> expected_tag_names =
        std::unordered_set<std::string>{"B", "C"};
    CHECK(contains_expected_tag_names(structure, expected_tag_names));

    tagB_ref.rename("D");
    expected_tag_names = std::unordered_set<std::string>{"C", "D"};
    CHECK(contains_expected_tag_names(structure, expected_tag_names));
  }

  SECTION("remove tag") {
    auto &tagB_ref = structure.addNested("B");
    auto &tagA_ref = structure.addNested("A");

    tagA_ref.remove();
    std::unordered_set<std::string> expected_tag_names =
        std::unordered_set<std::string>{"B"};
    CHECK(contains_expected_tag_names(structure, expected_tag_names));

    tagB_ref.remove();
    CHECK(structure.getNested().empty());
  }
}
