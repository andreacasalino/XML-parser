#include <Parser.h>
#include <iostream>
using namespace std;

int main() {
	//create a structure with an empty root
	xmlPrs::Parser parser;
	//print the entire structure into prompt
	cout << "\n\n\n The created structure is: \n";
	cout << parser << endl << endl;

	//rename the root
	parser.getRoot().setName("R");
	
	//add one field
	parser.getRoot().getAttributes().emplace("r", "1");

	//add some nested tags and then some attributes for them
	xmlPrs::Tag& tag1 = parser.getRoot().addNested("C1");
	tag1.getAttributes().emplace("A", "a");
	tag1.getAttributes().emplace("B", "b");

	xmlPrs::Tag& tag2 = parser.getRoot().addNested("C2");
	tag2.getAttributes().emplace("C", "c");

	xmlPrs::Tag& tag3 = parser.getRoot().addNested("C3");
	tag3.getAttributes().emplace("D", "d");

	//print the entire structure into prompt
	cout << "\n\n\n The actual structure is: \n";
	cout << parser << endl << endl;

	//print the structure into a textual file
	parser.reprint("XML_creted_from_code.xml");

	return EXIT_SUCCESS;
}