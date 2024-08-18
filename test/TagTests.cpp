#include <gtest/gtest.h>

#include "Common.h"

#include <XML-Parser/Tag.h>
using namespace xmlPrs;

bool contains_nested_names(const Tag& subject, std::vector<std::string_view> expected_names) {
  std::vector<std::string_view> names;
  for(const auto& [name, node] : subject.getNested()) {
    names.push_back(name);
  }
  std::sort(expected_names.begin(), expected_names.end());
  std::sort(names.begin(), names.end());
  return expected_names == names;
}

struct TagTests_ConstructionAndInspection : ::testing::Test {
  Tag structure{"root"};
};

TEST_F(TagTests_ConstructionAndInspection,  empty_after_construction) {
    EXPECT_FALSE(structure.hasFather());
}

TEST_F(TagTests_ConstructionAndInspection,  add_nested) {
    std::vector<std::string_view> tag_names{"tag1", "tag2", "element", "element", "element", "ref"};

    for (const auto &name : tag_names) {
      auto& added = structure.emplaceNested(std::string{name.data(), name.size()});
      EXPECT_EQ(added.getName(), name);
    }

    EXPECT_TRUE(contains_nested_names(structure, tag_names));
}

TEST_F(TagTests_ConstructionAndInspection,  complex_structure) {
    const std::size_t depth = 5;
    const std::string dummy_attr_name = "dummy";
    const std::string dummy_attr_value = "value";

    Tag *cursor = &structure;
    std::filesystem::path path;
    for (std::size_t d = 0; d < depth; ++d) {
      cursor->emplaceNested("B");
      cursor = &cursor->emplaceNested("A");
      path /= "A";
    }
    cursor->getAttributes().emplace(dummy_attr_name, dummy_attr_value);

    auto &tag_ref = structure.getDescendant(path);
    auto dummy_attr = tag_ref.getAttributes().begin();
    EXPECT_EQ(dummy_attr->first, dummy_attr_name);
    EXPECT_EQ(dummy_attr->second, dummy_attr_value);
}

bool is_well_connected(const Tag &tag) {
  auto check_child = [&tag](const Tag &child){
    return child.getFather() == &tag &&
           is_well_connected(child);    
  };

  const auto& nested = tag.getNested();
  return std::find_if(nested.begin(), nested.end(), [&check_child](const auto& el){
    return !check_child(*el.second);
  }) == nested.end();
}

bool has_child(const Tag &father, const std::string &child) {
  return father.getNested().find(child) != father.getNested().end();
}

TEST(TagTests, copy) {
  const auto& root = test::get_T_structure();

  auto &tag_A = *root.getNested().find("A")->second;
  auto &tag_B = *root.getNested().find("B")->second;
  auto &tag_C = *root.getNested().find("C")->second;
  auto &tag_D = *tag_A.getNested().find("D")->second;
  auto &tag_E = *tag_D.getNested().find("E")->second;

  Tag root2("root2");
  auto &tag_A_copy = root2.emplaceNested("A_copied");
  tag_A_copy = tag_A;

  ASSERT_TRUE(is_well_connected(root));
  ASSERT_TRUE(is_well_connected(root2));

  ASSERT_TRUE(has_child(root, "A"));
  ASSERT_TRUE(has_child(root, "B"));
  ASSERT_TRUE(has_child(root, "C"));
  ASSERT_TRUE(has_child(tag_A, "D"));
  ASSERT_TRUE(has_child(tag_D, "E"));

  ASSERT_TRUE(has_child(root2, "A"));
  ASSERT_TRUE(has_child(tag_A_copy, "D"));
  ASSERT_TRUE(has_child(*tag_A_copy.getNested().find("D")->second, "E"));
}

struct TagTests_Modification : ::testing::Test {
  Tag structure{"root"};
  Tag &tagB_ref = structure.emplaceNested("B");
  Tag &tagA_ref = structure.emplaceNested("A");
};

TEST_F(TagTests_Modification, rename) {
  tagA_ref.rename("C");
  EXPECT_TRUE(contains_nested_names(structure, std::vector<std::string_view>{"B", "C"}));

  tagB_ref.rename("D");
  EXPECT_TRUE(contains_nested_names(structure, std::vector<std::string_view>{"C", "D"}));
}

TEST_F(TagTests_Modification, modification) {
  tagA_ref.remove();
  EXPECT_TRUE(contains_nested_names(structure, std::vector<std::string_view>{"B"}));

  tagB_ref.remove();
  EXPECT_TRUE(structure.getNested().empty());
}
