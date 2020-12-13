#include <TagHandler.h>
#include <iostream>
using namespace std;

int main() {
	//create a structure with an empty root
	xmlPrs::Parser parser;
	//print the entire structure into prompt
	cout << "\n\n\n The created structure is: \n";
	cout << parser << endl << endl;

	//rename the root
	auto root = parser.GetRoot();
	root.SetTagName("R");
	
	//add one field
	root.AddAttribute("r", "1");

	//add some nested tags and then some attributes for them

	auto tag1 = root.AddNestedReturnCreated("C1");
	tag1.AddAttribute("A", "a");
	tag1.AddAttribute("B", "b");

	auto tag2 = root.AddNestedReturnCreated("C2");
	tag2.AddAttribute("C", "c");

	auto tag3 = tag2.AddNestedReturnCreated("C3");
	tag3.AddAttribute("D", "d");

	//print the entire structure into prompt
	cout << "\n\n\n The actual structure is: \n";
	cout << parser << endl << endl;

	//print the structure into a textual file
	parser.Reprint("XML_creted_from_code.xml");

	return 0;
}