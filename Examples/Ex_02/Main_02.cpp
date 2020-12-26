#include <Parser.h>
#include <iostream>
using namespace std;

int main() {
	string file = "XML_example_02.xml";

	//import the structure into a parser
	xmlPrs::Parser parser(file);
	//print the entire structure into prompt
	cout << "\n\n\n The parsed structure is: \n";
	cout << parser << endl << endl;

	//get tag at root->L1_1
	xmlPrs::Tag& L1 = *parser.getRoot().getNested("L1_1").begin()->second;
	// add to L1_1 an attribute called pippo, whose value is ciao
	L1.getAttributes().emplace("pippo", "ciao");
	//reprint the entire structure into prompt
	cout << "\n\n\n The actual structure is: \n";
	cout << parser << endl << endl;

	//get tag at root->L2
	xmlPrs::Tag& L2 = *parser.getRoot().getNested("L2").begin()->second;
	//remove L2
	L2.remove();
	//reprint the entire structure into prompt
	cout << "\n\n\n The actual structure is: \n";
	cout << parser << endl << endl;

	//reprint the new structure on a file
	parser.reprint("XML_example_02_modified.xml");

	return EXIT_SUCCESS;
}