#include <TagHandler.h>
#include <iostream>
using namespace std;

int main() {
	string file = "XML_example_02.xml";

	//import the structure into a parser
	xmlPrs::Parser parser(file);
	//print the entire structure into prompt
	cout << "\n\n\n The parsed structure is: \n";
	cout << parser << endl << endl;

	//get the root
	xmlPrs::TagHandler root = parser.GetRoot();

	//get tag at root->L1_1
	xmlPrs::TagHandler L1 = root.GetNestedFirst("L1_1");
	// add to L1_1 an attribute called pippo, whose value is ciao
	L1.AddAttribute("pippo", "ciao");
	//reprint the entire structure into prompt
	cout << "\n\n\n The actual structure is: \n";
	cout << parser << endl << endl;

	//get tag at root->L2
	xmlPrs::TagHandler L2 = root.GetNestedFirst("L2");
	//remove L2
	L2.Remove();
	//reprint the entire structure into prompt
	cout << "\n\n\n The actual structure is: \n";
	cout << parser << endl << endl;

	//reprint the new structure on a file
	parser.Reprint("XML_example_02_modified.xml");

	return 0;
}