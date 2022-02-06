#include <XML-Parser/Parser.h>
#include <iostream>
using namespace std;

int main() {
  string file = std::string(SAMPLE_FOLDER) + std::string("XML_example_01.xml");

  // import the structure into a parser
  auto root_parsed = xmlPrs::parse_xml(file);
  // access the root content, supposing the parse function suceeded
  xmlPrs::Root &root = std::get<xmlPrs::Root>(root_parsed);

  // get tag at root->L1_1
  xmlPrs::Tag &L1_1 = *root.getNested().find("L1_1")->second;
  // display fields of root->L1_1
  cout << "The attributes inside L1_1 are (name - value):\n";
  for (const auto field_it : L1_1.getAttributes()) {
    cout << field_it.first << "  -  " << field_it.second << endl;
  }

  // get tag at root->L1_3->L2_2->L3_2
  xmlPrs::Tag &L3_2 = root.getDescendant({"L1_3", "L2_2", "L3_2"});
  // get all fields of root->L1_1 and display it
  cout << "The attributes inside L3_2 are (name - value):\n";
  for (const auto field_it : L3_2.getAttributes()) {
    cout << field_it.first << "  -  " << field_it.second << endl;
  }

  // get tag at root->L1_4->L2_2
  xmlPrs::Tag &L2_2 = root.getDescendant({"L1_4", "L2_2"});
  // get the value of an attribute
  string field_L2_2 = L2_2.getAttributes().find("word")->second;

  // check about the existance of nested fields or tags
  cout << "Does L2_2 contain an attribute with name = dummy_name ?\n";
  if (L2_2.getNested().find("dummy_name") != L2_2.getNested().end())
    cout << "yes\n";
  else
    cout << "no\n";

  cout << "Does the root contain a nested tag with name = L1_6 ?\n";
  if (root.getNested().find("L1_6") != root.getNested().end())
    cout << "yes\n";
  else
    cout << "no\n";

  cout << "Does the root contain a nested tag with name = L1_2 ?\n";
  if (root.getNested().find("L1_2") != root.getNested().end())
    cout << "yes\n";
  else
    cout << "no\n";

  // print the entire structure into prompt
  cout << "\n\n\n The parsed structure is: \n";
  cout << root << endl;

  // remove tag L3_3
  root.getDescendant({"L1_3", "L2_2", "L3_3"}).remove();
  cout << "\n\n\n structure modified: \n";
  cout << root << endl;

  return EXIT_SUCCESS;
}
