#include "../../XML_Manager/XML_Manager.h"
#include <iostream>
using namespace std;


int main() {
	string file = "XML_example_01.xml";
	list<string> fields_name;
	list<string> fields_value;

	//import the structure into an XML_reader
	XML_reader parser(file);

	//get ref to root
	XML_reader::Tag_readable tag_root = parser.Get_root();

	//get tag at root->L1_1
	XML_reader::Tag_readable L1_1 = tag_root.Get_Nested_first_found("L1_1");
	//get all fields of root->L1_1 and display it
	L1_1.Get_all_Attributes(&fields_name, &fields_value);
	cout << "The attributes inside L1_1 are (name - value):\n";
	auto it_val = fields_value.begin();
	for (auto it = fields_name.begin(); it != fields_name.end(); it++) {
		cout << *it << "  -  " << *it_val << endl;
		it_val++;
	}

	//get tag at root->L1_3->L2_2->L3_2
	XML_reader::Tag_readable L3_2 = tag_root.Get_Nested(list<string>({ "L1_3", "L2_2", "L3_2" }));
	//get all fields of root->L1_1 and display it
	L3_2.Get_all_Attributes(&fields_name, &fields_value);
	cout << "The attributes inside L3_2 are (name - value):\n";
	it_val = fields_value.begin();
	for (auto it = fields_name.begin(); it != fields_name.end(); it++) {
		cout << *it << "  -  " << *it_val << endl;
		it_val++;
	}


	//get tag at root->L1_4->L2_2
	XML_reader::Tag_readable L2_2 = tag_root.Get_Nested(list<string>({ "L1_4", "L2_2" }));
	//get the value of an attribute
	string field_L2_2(*L2_2.Get_Attribute_first_found("word"));

	//check about the existance of nested fields or tags
	cout << "Does L2_2 contain an attribute with name = dummy_name ?\n";
	if (L2_2.Exist_Field("dummy_name")) cout << "yes\n";
	else cout << "no\n";

	cout << "Does the root contain a nested tag with name = L1_6 ?\n";
	if (tag_root.Exist_Nested_tag("L1_6")) cout << "yes\n";
	else cout << "no\n";

	cout << "Does the root contain a nested tag with name = L1_2 ?\n";
	if (tag_root.Exist_Nested_tag("L1_2")) cout << "yes\n";
	else cout << "no\n";

	//print the entire structure into prompt
	cout << "\n\n\n The parsed structure is: \n";
	parser.Reprint(cout);
	cout << endl;

	system("pause");
	return 0;
}