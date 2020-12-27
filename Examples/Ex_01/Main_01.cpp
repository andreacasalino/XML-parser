#include <Parser.h>
#include <iostream>
using namespace std;

int main() {
	string file = "XML_example_01.xml";
	std::multimap<std::string, std::string>* fields = nullptr;

	//import the structure into a parser
	xmlPrs::Parser parser(file);

	//get tag at root->L1_1
	xmlPrs::Tag& L1_1 = *parser.getRoot().getNested("L1_1").begin()->second;
	//get all fields of root->L1_1 and display it
	fields = &L1_1.getAttributes();
	cout << "The attributes inside L1_1 are (name - value):\n";
	for (auto it = fields->begin(); it != fields->end(); it++) {
		cout << it->first << "  -  " << it->second << endl;
	}

	//get tag at root->L1_3->L2_2->L3_2
	xmlPrs::Tag& L3_2 = parser.getRoot().getNested(vector<string>{ "L1_3", "L2_2", "L3_2" });
	//get all fields of root->L1_1 and display it
	fields = &L3_2.getAttributes();
	cout << "The attributes inside L3_2 are (name - value):\n";
	for (auto it = fields->begin(); it != fields->end(); it++) {
		cout << it->first << "  -  " << it->second << endl;
	}

	//get tag at root->L1_4->L2_2
	xmlPrs::Tag& L2_2 = parser.getRoot().getNested(vector<string>{ "L1_4", "L2_2" });
	//get the value of an attribute
	string field_L2_2 = L2_2.getAttributes().find("word")->second;

	//check about the existance of nested fields or tags
	cout << "Does L2_2 contain an attribute with name = dummy_name ?\n";
	if (L2_2.getNested("dummy_name").size() > 0) cout << "yes\n";
	else cout << "no\n";

	cout << "Does the root contain a nested tag with name = L1_6 ?\n";
	if (parser.getRoot().getNested("L1_6").size() > 0) cout << "yes\n";
	else cout << "no\n";

	cout << "Does the root contain a nested tag with name = L1_2 ?\n";
	if (parser.getRoot().getNested("L1_2").size() > 0) cout << "yes\n";
	else cout << "no\n";

	//print the entire structure into prompt
	cout << "\n\n\n The parsed structure is: \n";
	cout << parser << endl;

	//print tag L2_2 alone
	cout << "\n\n\n content of tag L2_2: \n";
	cout << parser.getRoot().getNested(std::vector<std::string>{"L1_3", "L2_2"}) << endl;

	//remove tag L3_3
	parser.getRoot().getNested(vector<string>{"L1_3", "L2_2", "L3_3"}).remove();
	cout << "\n\n\n structure modified: \n";
	cout << parser << endl;
	
	return EXIT_SUCCESS;
}

