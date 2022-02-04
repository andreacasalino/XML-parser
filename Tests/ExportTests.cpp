#include <catch2/catch_test_macros.hpp>

#include "T_structure.h"
#include <XML-Parser/Parser.h>
#include <fstream>

using namespace xmlPrs;

TEST_CASE("Xml export", "[export]") {
  auto root = generate_T_structure();

  static const std::string temp_file = "temp.xml";
  {
    std::ofstream stream(temp_file);
    stream << root;
  }

  auto imported = parse_xml(temp_file);
  // CHECK(std::get_if<Error>(&imported) == nullptr);
  // CHECK(is_valid_T_structure(std::get<Root>(imported)));
}
