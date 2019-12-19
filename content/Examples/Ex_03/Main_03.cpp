#include "../../XML_Manager/XML_Manager.h"
#include <iostream>
using namespace std;



int main() {

	//create a structure with an empty root
	XML_reader parser;
	//print the entire structure into prompt
	cout << "\n\n\n The created structure is: \n";
	parser.Reprint(cout);
	cout << endl;

	//rename the root
	auto root = parser.Get_root();
	root.Set_tag_name("R");
	
	//add one field
	root.Add_field("r", "1");

	//add some nested tags and then some sttributes for them

	auto tag1 = root.Add_Nested_and_return_created("C1");
	tag1.Add_field("A", "a");
	tag1.Add_field("B", "b");

	auto tag2 = root.Add_Nested_and_return_created("C2");
	tag2.Add_field("C", "c");

	//print the entire structure into prompt
	cout << "\n\n\n The actual structure is: \n";
	parser.Reprint(cout);
	cout << endl;

	//print the structure into a textual file
	parser.Reprint("XML_creted_from_code.xml");

	system("pause");
	return 0;
}