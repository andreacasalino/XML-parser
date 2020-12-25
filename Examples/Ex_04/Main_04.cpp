#include <TagHandler.h>
#include <iostream>
using namespace std;

int main() {
	xmlPrs::Parser structure;
	auto root = structure.GetRoot();

	{
		xmlPrs::Parser temp("XML_example_04a.xml");
		auto addTag = root.AddNestedStructure(temp.GetRoot());
		cout << endl << endl << "first file content" << endl;
		cout << addTag;
	}

	{
		xmlPrs::Parser temp("XML_example_04b.xml");
		auto addTag = root.AddNestedStructure(temp.GetRoot());
		cout << endl << endl << "second file content" << endl;
		cout << addTag;
	}

	//print the entire structure into prompt
	cout << "\n\n\n The first structure is: \n";
	cout << structure << endl;

	return EXIT_SUCCESS;
}


