#include <gtest/gtest.h>

#include "Common.h"
#include <XML-Parser/Parser.h>

#include <fstream>

using namespace xmlPrs;

struct ExportTests : ::testing::Test {
  test::TmpFile fldr{"ExportTests"};
  std::filesystem::path filename{fldr.get() / "tmp.xml"};
};

TEST_F(ExportTests, export_then_import) {
  {
    std::ofstream stream(filename);
    stream << test::get_T_structure();
  }

  auto [_, imported] = parseFile(filename);
  EXPECT_EQ(*imported, test::get_T_structure());
}
