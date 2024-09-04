#include <gtest/gtest.h>

#include "Common.h"
#include <XML-Parser/Parser.h>
using namespace xmlPrs;

namespace {
std::filesystem::path testFileName(std::string filename) {
  filename += ".xml";
  std::filesystem::path res = TEST_DATA;
  res /= filename;
  return res;
}
} // namespace

TEST(ParserTests_Invalid, invalidFile) {
  auto path = testFileName("Inexistent");
  EXPECT_THROW(parseFile(path), std::runtime_error);
}

struct ParseFileTStructureTestFixture
    : ::testing::TestWithParam<std::filesystem::path> {};
TEST_P(ParseFileTStructureTestFixture, parse) {
  auto [_, parsed] = parseFile(GetParam());
  ASSERT_TRUE(parsed);
  EXPECT_TRUE(test::get_T_structure() == *parsed);
}
INSTANTIATE_TEST_CASE_P(ParserTests_TStructure, ParseFileTStructureTestFixture,
                        ::testing::Values(testFileName("TStructure.A"),
                                          testFileName("TStructure.B"),
                                          testFileName("TStructure.C"),
                                          testFileName("TStructure.D")));

struct ParserTests : ::testing::Test {
  void TearDown() override {
    auto [_, parsed] = parseFile(filename);
    ASSERT_TRUE(parsed);
    EXPECT_TRUE(*expected == *parsed);
  }

  TagPtr makeTag() { return std::make_unique<Tag>("Tag"); }

  std::filesystem::path filename;
  TagPtr expected;
};

TEST_F(ParserTests, chain) {
  filename = testFileName("Chain");

  expected = makeTag();
  for (std::size_t k = 0; k < 3; ++k) {
    expected->addNested(makeTag());
  }
}

TEST_F(ParserTests, recursive) {
  filename = testFileName("Recursive");

  expected = makeTag();
  expected->addNested(makeTag()).addNested(makeTag()).addNested(makeTag());
  expected->addNested(makeTag()).addNested(makeTag()).addNested(makeTag());
}

TEST_F(ParserTests, content) {
  filename = testFileName("Content");

  expected = makeTag();
  {
    auto nested = makeTag();
    nested->setContent("first content");
    expected->addNested(std::move(nested));
  }
  {
    auto nested = makeTag();
    nested->setContent("second content");
    expected->addNested(std::move(nested));
  }
}
