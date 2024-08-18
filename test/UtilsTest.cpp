#include <gtest/gtest.h>

#include <Utils.h>

TEST(UtilsTest, positive) {
  bool flag = xmlPrs::match<',', '\n'>(',');
  EXPECT_TRUE(flag);
}

TEST(UtilsTest, negative) {
  bool flag = xmlPrs::match<',', '\n'>('$');
  EXPECT_FALSE(flag);
}

struct UtilsTest_StripTest : ::testing::Test {
  std::string_view buffer{" . ..  .. .. .. Hello World. ..  .. .. .. "};
};

TEST_F(UtilsTest_StripTest, strip_left) {
  xmlPrs::stripL<' ', '.'>(buffer);
  EXPECT_EQ(buffer, "Hello World. ..  .. .. .. ");
}

TEST_F(UtilsTest_StripTest, strip_right) {
  xmlPrs::stripR<' ', '.'>(buffer);
  EXPECT_EQ(buffer, " . ..  .. .. .. Hello World");
}

struct UtilsTestFrontMatchTestFixture
    : ::testing::TestWithParam<std::string_view> {};
TEST_P(UtilsTestFrontMatchTestFixture, single_no_rest) {
  auto res = xmlPrs::doesFrontMatch(GetParam(), 'a');
  ASSERT_TRUE(res.first);
  EXPECT_TRUE(res.second.empty());
}
INSTANTIATE_TEST_CASE_P(UtilsTest, UtilsTestFrontMatchTestFixture,
                        ::testing::Values("a", "  a", "\na", "\ta", " \n\ta"));

struct UtilsTestFrontMatchWithRestTestFixture
    : ::testing::TestWithParam<std::string_view> {};
TEST_P(UtilsTestFrontMatchWithRestTestFixture, single_with_rest) {
  auto res = xmlPrs::doesFrontMatch(GetParam(), 'a');
  ASSERT_TRUE(res.first);
  EXPECT_EQ(res.second, "the rest");
}
INSTANTIATE_TEST_CASE_P(UtilsTest, UtilsTestFrontMatchWithRestTestFixture,
                        ::testing::Values("athe rest", "  athe rest",
                                          "\nathe rest", "\tathe rest",
                                          " \n\tathe rest"));

struct UtilsTestFrontMatchMultipleTestFixture
    : ::testing::TestWithParam<std::string_view> {};
TEST_P(UtilsTestFrontMatchMultipleTestFixture, single_no_rest) {
  auto res = xmlPrs::doesFrontMatch(GetParam(), 'a', "bbb", 'c', "dd");
  ASSERT_TRUE(res.first);
  EXPECT_TRUE(res.second.empty());
}
INSTANTIATE_TEST_CASE_P(UtilsTest, UtilsTestFrontMatchMultipleTestFixture,
                        ::testing::Values("a bbb c dd", "a   bbb c  dd",
                                          "a bbbc  dd", R"(a 
                                          bbb c  
                                          dd)"));

TEST(UtilsTest, full_removal) {
  std::string_view buffer{"Hello World"};
  auto cut = xmlPrs::cut<'~'>(buffer);
  EXPECT_EQ(cut, "Hello World");
  EXPECT_TRUE(buffer.empty());
}

TEST(UtilsTest, one_delimeter) {
  std::string_view buffer{"Hello World~/"};
  auto cut = xmlPrs::cut<'~'>(buffer);
  EXPECT_EQ(cut, "Hello World");
  EXPECT_EQ(buffer, "~/");
}

TEST(UtilsTest, multiple_delimeters) {
  std::string_view buffer{"Hello World~/"};
  auto cut = xmlPrs::cut<'/', '~'>(buffer);
  EXPECT_EQ(cut, "Hello World");
  EXPECT_EQ(buffer, "~/");
}

TEST(UtilsTest, push_some_values) {
  std::vector<std::string_view> values;
  values.emplace_back();

  xmlPrs::ViewsCollection collection;
  collection += "Hello ";
  collection += "World";
  EXPECT_EQ(collection.toString(), "Hello World");
}

struct ViewsCollectionTestFixture
    : ::testing::TestWithParam<std::vector<std::string_view>> {};

TEST_P(ViewsCollectionTestFixture, emptyConvertedString) {
  xmlPrs::ViewsCollection collection;
  for (const auto &str : GetParam()) {
    collection += str;
  }
  EXPECT_TRUE(collection.toString().empty());
}
INSTANTIATE_TEST_CASE_P(
    UtilsTest, ViewsCollectionTestFixture,
    ::testing::Values(std::vector<std::string_view>{},
                      std::vector<std::string_view>{"  ", " "},
                      std::vector<std::string_view>{"\n\n", "\n"},
                      std::vector<std::string_view>{" \n ", "\n\n "}));
