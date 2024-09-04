#include <XML-Parser/Parser.h>

#include <filesystem>
#include <fstream>
#include <iostream>
using namespace std;

int main() {
  std::filesystem::path filename =
      std::filesystem::path{SAMPLE_FOLDER} / "XML_example_02.xml";

  // import the structure into a parser
  auto [metaData, root] = xmlPrs::parseFile(filename);

  // print the entire structure into prompt
  cout << "\n\n\n The parsed structure is: \n";
  cout << *root << endl << endl;

  // get tag at root / L1_1
  xmlPrs::Tag &L1 = *root->getNested().find("L1_1")->second;
  // add to L1_1 an attribute called pippo, whose value is ciao
  L1.getAttributes().emplace("pippo", "ciao");
  // reprint the entire structure into prompt
  cout << "\n\n\n The actual structure is: \n";
  cout << *root << endl << endl;

  // get tag at root / L2
  xmlPrs::Tag &L2 = *root->getNested().find("L2")->second;
  // remove L2
  L2.remove();
  // reprint the entire structure into prompt
  cout << "\n\n\n The actual structure is: \n";
  cout << *root << endl << endl;

  // reprint the new structure on a file
  std::string reprintFilename = "XML_example_02_modified.xml";
  std::ofstream stream(reprintFilename);
  stream << *root;
  std::cout << "Check what was written to: "
            << std::filesystem::current_path() / reprintFilename << std::endl;

  return EXIT_SUCCESS;
}