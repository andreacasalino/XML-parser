#include <catch2/catch_test_macros.hpp>
#include <catch2/generators/catch_generators.hpp>

#include <unordered_set>

#include <XML-Parser/Tag.h>
using namespace xmlPrs;

TEST_CASE("Xml construction and inspection", "[generation]") {
  Root structure;

  SECTION("root inspection") { CHECK_FALSE(structure.hasFather()); }

  SECTION("attributes generation") {
    std::unordered_multimap<std::string, std::string> attributes;
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
    const auto &nested = structure.getNested();
    for (const auto &name : tag_names) {
      auto nested_it = nested.find(name);
      REQUIRE(nested_it != nested.end());
      CHECK(&nested_it->second->getFather() == &structure);
    }
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

    std::vector<std::string> path(depth, "A");
    auto &tag_ref = structure.getDescendant(path);
    auto dummy_attr = tag_ref.getAttributes().begin();
    CHECK(dummy_attr->first == dummy_attr_name);
    CHECK(dummy_attr->second == dummy_attr_value);
  }
}

TEST_CASE("Tag copy", "[generation]") {}

TEST_CASE("Tag move", "[generation]") {}

TEST_CASE("Xml modification", "[generation]") {}
