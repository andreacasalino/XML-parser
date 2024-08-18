#include <XML-Parser/Parser.h>
#include <fstream>
#include <iostream>
using namespace std;

int main() {
  // create a structure with an empty root
  xmlPrs::Tag root{"Root"};
  // print the entire structure into prompt
  cout << "\n\n\n The created structure is: \n";
  cout << root << endl << endl;

  // add one field
  root.getAttributes().emplace("r", "1");

  // add some nested tags and then some attributes for them
  xmlPrs::Tag &tag1 = root["C1"];
  tag1.getAttributes().emplace("A", "a");
  tag1.getAttributes().emplace("B", "b");

  xmlPrs::Tag &tag2 = root["C2"];
  tag2.getAttributes().emplace("C", "c");

  xmlPrs::Tag &tag3 = root["C3"];
  tag3.getAttributes().emplace("D", "d");

  // print the entire structure into prompt
  cout << "\n\n\n The actual structure is: \n";
  cout << root << endl << endl;

  // print the structure into a textual file
  std::string reprintFilename = "XML_creted_from_code.xml";
  std::ofstream stream(reprintFilename);
  stream << root;
  std::cout << "Check what was written to: " << std::filesystem::current_path() / reprintFilename << std::endl;

  return EXIT_SUCCESS;
}