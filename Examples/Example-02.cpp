#include <XML-Parser/Tag.h>
#include <fstream>
#include <iostream>
using namespace std;

int main() {
  string file = std::string(SAMPLE_FOLDER) + std::string("XML_example_02.xml");

  // import the structure into a parser
  auto root_parsed = xmlPrs::parse_xml(file);
  // access the root content, supposing the parse function suceeded
  xmlPrs::Root &root = std::get<xmlPrs::Root>(root_parsed);

  // print the entire structure into prompt
  cout << "\n\n\n The parsed structure is: \n";
  cout << root << endl << endl;

  // get tag at root->L1_1
  xmlPrs::Tag &L1 = *root.getNested().find("L1_1")->second;
  // add to L1_1 an attribute called pippo, whose value is ciao
  L1.getAttributes().emplace("pippo", "ciao");
  // reprint the entire structure into prompt
  cout << "\n\n\n The actual structure is: \n";
  cout << root << endl << endl;

  // get tag at root->L2
  xmlPrs::Tag &L2 = *root.getNested().find("L2")->second;
  // remove L2
  L2.remove();
  // reprint the entire structure into prompt
  cout << "\n\n\n The actual structure is: \n";
  cout << root << endl << endl;

  // reprint the new structure on a file
  std::ofstream stream("XML_example_02_modified.xml");
  stream << root;

  return EXIT_SUCCESS;
}