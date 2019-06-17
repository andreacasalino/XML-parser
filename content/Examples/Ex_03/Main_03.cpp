#include "../../XML_Manager/XML_Manager.h"
#include <iostream>
using namespace std;



int main() {
	system("COLOR 0A");
	system("ECHO This example creates from code lines an XML and the print it");

	XML_reader parser; //in this way, it is created an xml containing only the root with name Root

	//rename the root
	auto root = parser.Get_root();
	root.Set_name("R");
	
	//add one field
	root.Add_field("r", "1");

	//add two nested tags
	auto tag1 = root.Add_Nested_and_return_created("C1");
	tag1.Add_field("A", "a");
	tag1.Add_field("B", "b");

	auto tag2 = root.Add_Nested_and_return_created("C2");
	tag2.Add_field("C", "c");

	//print the resulting xml
	parser.Reprint(cout);
	cout << endl;

	parser.Reprint("XML_creted_from_code.xml");

	system("pause");
	return 0;
}