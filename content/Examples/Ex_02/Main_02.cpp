#include "../../XML_Manager/XML_Manager.h"
#include <iostream>
using namespace std;


int main() {

	string file = "XML_example_02.xml";

	//import the structure into an XML_reader
	XML_reader parser(file);
	//print the entire structure into prompt
	cout << "\n\n\n The parsed structure is: \n";
	parser.Reprint(cout);
	cout << endl << endl;

	//get the root
	XML_reader::Tag_readable root = parser.Get_root();

	//get tag at root->L1_1
	XML_reader::Tag_readable L1 = root.Get_Nested_first_found("L1_1");
	// add to L1_1 an attribute called pippo, whose value is ciao
	L1.Add_field("pippo", "ciao");
	//reprint the entire structure into prompt
	cout << "\n\n\n The actual structure is: \n";
	parser.Reprint(cout);
	cout << endl << endl;

	//get tag at root->L2
	XML_reader::Tag_readable L2 = root.Get_Nested_first_found("L2");
	//remove L2
	L2.Remove();
	//reprint the entire structure into prompt
	cout << "\n\n\n The actual structure is: \n";
	parser.Reprint(cout);
	cout << endl << endl;

	//reprint the new structure on a file
	parser.Reprint("XML_example_02_modified.xml");

	system("pause");
	return 0;
}