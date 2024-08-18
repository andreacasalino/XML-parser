#include <XML-Parser/Parser.h>
#include <iostream>
using namespace std;

int main() {
  xmlPrs::TagPtr root;

  {
    std::filesystem::path filename =
        std::filesystem::path{SAMPLE_FOLDER} / "XML_example_04a.xml";
    auto [metaData, root] = xmlPrs::parseFile(filename);
    cout << endl << endl << "first file content" << endl;
    cout << *root;
  }

  {
    std::filesystem::path filename =
        std::filesystem::path{SAMPLE_FOLDER} / "XML_example_04b.xml";
    auto [metaData, root] = xmlPrs::parseFile(filename);
    cout << endl << endl << "second file content" << endl;
    cout << *root;
  }

  // print the entire structure into prompt
  cout << "\n\n\n The complete structure now is: \n";
  cout << *root << endl;

  // rename some tags
  root->getDescendant({"R0", "R1_1"}).rename("R1_1_renamed");
  root->getDescendant({"R1", "R1_3"}).rename("R1_3_renamed");
  cout << "\n\n\n The modified structure: \n";
  cout << *root << endl;

  return EXIT_SUCCESS;
}
