#include <XML-Parser/Parser.h>

#include <filesystem>
#include <iostream>
using namespace std;

int main() {
  std::filesystem::path filename =
      std::filesystem::path{SAMPLE_FOLDER} / "XML_example_01.xml";

  // import the structure into a parser
  auto [metaData, root] = xmlPrs::parseFile(filename);
  std::cout << "Found version: " << metaData["version"] << std::endl;

  // get tag at root / L1_1
  xmlPrs::Tag &L1_1 = *root->getNested().find("L1_1")->second;
  // display fields of root / L1_1
  std::cout << "The attributes inside L1_1 are (name - value):\n";
  for (const auto field_it : L1_1.getAttributes()) {
    std::cout << field_it.first << "  -  " << field_it.second << endl;
  }

  // get tag at root / L1_3 / L2_2 / L3_2
  xmlPrs::Tag &L3_2 =
      root->getDescendant(std::filesystem::path{"L1_3"} / "L2_2" / "L3_2");
  // get all fields of root / L1_1 and display it
  std::cout << "The attributes inside L3_2 are (name - value):\n";
  for (const auto field_it : L3_2.getAttributes()) {
    std::cout << field_it.first << "  -  " << field_it.second << endl;
  }

  // get tag at root / L1_4 / L2_2
  xmlPrs::Tag &L2_2 =
      root->getDescendant(std::filesystem::path{"L1_4"} / "L2_2");
  // get the value of an attribute
  string field_L2_2 = L2_2.getAttributes().find("word")->second;

  // get content of root / L1_2
  std::cout << "content of tag at path root / L1_2:\n";
  std::cout << root->getDescendant(std::filesystem::path{"L1_2"}).getContent()
            << std::endl;

  // check about the existance of nested fields or tags
  std::cout << "Does L2_2 contain an attribute with name = dummy_name ?\n";
  if (L2_2.getNested().find("dummy_name") != L2_2.getNested().end())
    std::cout << "yes\n";
  else
    std::cout << "no\n";

  std::cout << "Does the root contain a nested tag with name = L1_6 ?\n";
  if (root->getNested().find("L1_6") != root->getNested().end())
    std::cout << "yes\n";
  else
    std::cout << "no\n";

  std::cout << "Does the root contain a nested tag with name = L1_2 ?\n";
  if (root->getNested().find("L1_2") != root->getNested().end())
    std::cout << "yes\n";
  else
    std::cout << "no\n";

  // print the entire structure into prompt
  std::cout << "\n\n\n The parsed structure is: \n";
  std::cout << *root << endl;

  // remove tag L3_3
  root->getDescendant(std::filesystem::path{"L1_3"} / "L2_2" / "L3_3").remove();
  std::cout << "\n\n\n structure modified: \n";
  std::cout << *root << endl;

  return EXIT_SUCCESS;
}
