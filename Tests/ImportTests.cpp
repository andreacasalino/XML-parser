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

TEST_CASE("Xml construction and inspection", "[generation]") {
  SECTION("invalid files") {
    auto file_location = GENERATE(make_file_location("Inexistent"),
                                  make_file_location("Invalid"));

    auto parsed = parse_xml(file_location);
    CHECK(std::get_if<Error>(&parsed) != nullptr);
  }

  SECTION("valid files") {
    auto file_location = GENERATE(make_file_location("ImportTest"),
                                  make_file_location("ImportTestVersioned"));

    auto parsed = parse_xml(file_location);
    CHECK(std::get_if<Error>(&parsed) == nullptr);
    auto &parsed_root = std::get<Root>(parsed);

    CHECK(is_valid_T_structure(parsed_root));
  }
}
