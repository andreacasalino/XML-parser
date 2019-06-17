#include "../../XML_Manager/XML_Manager.h"
#include <iostream>
#include <Windows.h> // This is included just for using IsDebuggerPresent, in order to understand where is the actual directory
using namespace std;



int main() {
	system("COLOR 0A");
	system("ECHO This example reads a simple xml file and manipulates some of the nested values");

	string file = "XML_example_02.xml";
	if (!IsDebuggerPresent()) file = "../../Example/" + file;
	XML_reader parser(file);
	XML_reader::Tag_readable root = parser.Get_root();
	parser.Reprint(cout);
	cout << endl << endl << endl;

	XML_reader::Tag_readable L1 = root.Get_Nested("L1_1");
	L1.Add_field("pippo", "ciao");
	parser.Reprint(cout);
	cout << endl << endl << endl;

	XML_reader::Tag_readable L2 = root.Get_Nested("L2");
	L2.Remove();
	parser.Reprint(cout);
	cout << endl << endl << endl;

	parser.Reprint("XML_example_02_modified.xml");

	system("pause");
	return 0;
}