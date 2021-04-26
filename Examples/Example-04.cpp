#include <Parser.h>
#include <iostream>
using namespace std;

int main() {
	xmlPrs::Tag structure("R");

	{
		xmlPrs::Parser temp(std::string(SAMPLE_FOLDER) + std::string("XML_example_04a.xml"));
		xmlPrs::Tag& addTag = structure.addNested(std::move(temp.getRoot()));
		cout << endl << endl << "first file content" << endl;
		cout << addTag;
	}

	{
		xmlPrs::Parser temp(std::string(SAMPLE_FOLDER) + std::string("XML_example_04b.xml"));
		xmlPrs::Tag& addTag = structure.addNested(std::move(temp.getRoot()));
		cout << endl << endl << "second file content" << endl;
		cout << addTag;
	}

	//print the entire structure into prompt
	cout << "\n\n\n The complete structure now is: \n";
	cout << structure << endl;

	//rename some tags
	structure.getNested(vector<string>{"R0" , "R1_1"}).setName("R1_1_renamed");
	structure.getNested(vector<string>{"R1" , "R1_3"}).setName("R1_3_renamed");
	cout << "\n\n\n The renamed structure now is: \n";
	cout << structure << endl;

	return EXIT_SUCCESS;
}


