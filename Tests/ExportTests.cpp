#include <catch2/catch_test_macros.hpp>
#include <catch2/generators/catch_generators.hpp>

#include <fstream>
#include <unordered_set>

#include <XML-Parser/Tag.h>
using namespace xmlPrs;

TEST_CASE("Xml exporting test", "[export]") {
  Root structure;

  const std::size_t depth = GENERATE(2, 5, 10);

  SECTION("check on linear structure") {
    Tag *cursor = &structure;
    for (std::size_t d = 0; d < depth; ++d) {
      cursor = &cursor->addNested("A");
    }

    {
      std::ofstream stream("export_" + std::to_string(depth) + ".xml");
      stream << structure;
    }

    // TODO check what??
  }

  // TODO which other structure to check?
}
