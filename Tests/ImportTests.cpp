#include <catch2/catch_test_macros.hpp>
#include <catch2/generators/catch_generators.hpp>

#include "T_structure.h"
#include <sstream>

#include <XML-Parser/Parser.h>
using namespace xmlPrs;

std::string make_file_location(const std::string &file_name) {
  std::stringstream stream;
  stream << TEST_FOLDER << file_name << ".xml";
  return stream.str();
}

TEST_CASE("Xml parsing invalid files", "[import][invalid]") {
  auto file_location =
      GENERATE(make_file_location("Inexistent"), make_file_location("Invalid"));

  auto parsed = parse_xml(file_location);
  CHECK(std::get_if<Error>(&parsed) != nullptr);
}

TEST_CASE("Xml parsing valid files", "[import][valid]") {
    SECTION("T structure") {
        auto file_location = GENERATE(make_file_location("ImportTest"),
                                      make_file_location("ImportTestVersioned"),
                                      make_file_location("ImportTestComment"));

        auto parsed = parse_xml(file_location);
        CHECK(std::get_if<Error>(&parsed) == nullptr);
        auto& parsed_root = std::get<Root>(parsed);

        CHECK(is_valid_T_structure(parsed_root));
    }

    SECTION("misc") {
        auto file_location = GENERATE(make_file_location("ImportChain"),
                                      make_file_location("ImportRecursive"));

        auto parsed = parse_xml(file_location);
        CHECK(std::get_if<Error>(&parsed) == nullptr);
    }
}
