// #include <TagHandler.h>
// #include <iostream>
// using namespace std;

// int main() {
// 	string file = "XML_example_01.xml";
// 	std::vector<std::pair<std::string, std::string>> fields;

// 	//import the structure into a parser
// 	xmlPrs::Parser parser(file);

// 	//get ref to root
// 	xmlPrs::TagHandler tag_root = parser.GetRoot();

// 	//get tag at root->L1_1
// 	xmlPrs::TagHandler L1_1 = tag_root.GetNestedFirst("L1_1");
// 	//get all fields of root->L1_1 and display it
// 	fields = L1_1.GetAttributeAll();
// 	cout << "The attributes inside L1_1 are (name - value):\n";
// 	for (auto it = fields.begin(); it != fields.end(); it++) {
// 		cout << it->first << "  -  " << it->second << endl;
// 	}

// 	//get tag at root->L1_3->L2_2->L3_2
// 	xmlPrs::TagHandler L3_2 = tag_root.GetNested(vector<string>{ "L1_3", "L2_2", "L3_2" });
// 	//get all fields of root->L1_1 and display it
// 	fields = L3_2.GetAttributeAll();
// 	cout << "The attributes inside L3_2 are (name - value):\n";
// 	for (auto it = fields.begin(); it != fields.end(); it++) {
// 		cout << it->first << "  -  " << it->second << endl;
// 	}

// 	//get tag at root->L1_4->L2_2
// 	xmlPrs::TagHandler L2_2 = tag_root.GetNested(vector<string>{ "L1_4", "L2_2" });
// 	//get the value of an attribute
// 	string field_L2_2 = L2_2.GetAttributeValueFirst("word");

// 	//check about the existance of nested fields or tags
// 	cout << "Does L2_2 contain an attribute with name = dummy_name ?\n";
// 	if (L2_2.ExistAttribute("dummy_name")) cout << "yes\n";
// 	else cout << "no\n";

// 	cout << "Does the root contain a nested tag with name = L1_6 ?\n";
// 	if (tag_root.ExistNested("L1_6")) cout << "yes\n";
// 	else cout << "no\n";

// 	cout << "Does the root contain a nested tag with name = L1_2 ?\n";
// 	if (tag_root.ExistNested("L1_2")) cout << "yes\n";
// 	else cout << "no\n";

// 	//print the entire structure into prompt
// 	cout << "\n\n\n The parsed structure is: \n";
// 	cout << parser << endl;

// 	//print tag L2_2 alone
// 	cout << "\n\n\n content of tag L2_2: \n";
// 	cout << tag_root.GetNested(std::vector<std::string>{"L1_3", "L2_2"}) << endl;

// 	return EXIT_SUCCESS;
// }



#include <Tag.h>
#include <iostream>
using namespace std;

int main(){

	xmlPrs::Tag root("Root");

	root.getAttributes().emplace("f", "v0");
	root.getAttributes().emplace("f", "v1");
	root.getAttributes().emplace("f", "v2");
	root.getAttributes().emplace("f", "v1");
	root.getAttributes().emplace("f", "v1");
	cout << root << endl;

	root.setAttributeName("f", "f_new");
	cout << root << endl;

	root.setAttributeName("f_new", "v1", "f2");
	cout << root << endl;

	return EXIT_SUCCESS;
}

