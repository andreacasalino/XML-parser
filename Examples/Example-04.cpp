#include <XML-Parser/Parser.h>
#include <iostream>
using namespace std;

int main() {
  xmlPrs::Root root("R");

  {
    auto imported_root_raw = xmlPrs::parse_xml(
        std::string(SAMPLE_FOLDER) + std::string("XML_example_04a.xml"));
    auto &imported_root = std::get<xmlPrs::Root>(imported_root_raw);
    root[imported_root.getName()] = imported_root;
    cout << endl << endl << "first file content" << endl;
    cout << imported_root;
  }

  {
    auto imported_root_raw = xmlPrs::parse_xml(
        std::string(SAMPLE_FOLDER) + std::string("XML_example_04b.xml"));
    auto &imported_root = std::get<xmlPrs::Root>(imported_root_raw);
    root[imported_root.getName()] = imported_root;
    cout << endl << endl << "second file content" << endl;
    cout << imported_root;
  }

  // print the entire structure into prompt
  cout << "\n\n\n The complete structure now is: \n";
  cout << root << endl;

  // rename some tags
  root.getDescendant({"R0", "R1_1"}).rename("R1_1_renamed");
  root.getDescendant({"R1", "R1_3"}).rename("R1_3_renamed");
  cout << "\n\n\n The modified structure: \n";
  cout << root << endl;

  return EXIT_SUCCESS;
}
