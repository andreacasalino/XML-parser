#include "../../XML_Manager/XML_Manager.h"
#include <iostream>
#include <Windows.h> // This is included just for using IsDebuggerPresent, in order to understand where is the actual directory
using namespace std;



int main() {
	system("COLOR 0A");
	system("ECHO This example reads a simple xml file and manipulates some of the nested values");

	string file = "XML_example_01.xml";
	if (!IsDebuggerPresent()) file = "../../Example/" + file;
	XML_reader parser(file);

	list<string> fields_name;
	list<string> fields_value;



	//get ref to root
	XML_reader::Tag_readable tag_root = parser.Get_root();

	//get tag root->L1_1
	XML_reader::Tag_readable L1_1 = tag_root.Get_Nested("L1_1");
	//get all fields of root->L1_1
	L1_1.Get_all_values(&fields_name, &fields_value);



	//get tag root->L1_3->L2_2->L3_2
	XML_reader::Tag_readable L3_2 = tag_root.Get_Nested(list<string>({ "L1_3", "L2_2", "L3_2" }));
	L3_2.Get_all_values(&fields_name, &fields_value);


	//get tag root->L1_4->L2_2
	XML_reader::Tag_readable L2_2 = tag_root.Get_Nested(list<string>({ "L1_4", "L2_2" }));
	string field_L2_2 = L2_2.Get_value("word");


	//check about the existance of nested fields or tags
	bool existance_flag_01 = L2_2.Exist_Field("non_existing_field_name");
	bool existance_flag_02 = tag_root.Exist_Nested_tag("L1_6");
	bool existance_flag_03 = tag_root.Exist_Nested_tag("L1_2");


	parser.Reprint(cout);
	cout << endl;


	system("pause");
	return 0;
}