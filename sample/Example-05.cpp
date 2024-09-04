#include <XML-Parser/Converter.h>
#include <iostream>
using namespace std;

int main() {
	xmlPrs::Tag root("Root");
	
	{
		auto& A = root.emplaceNested("A");
		A.getAttributes().emplace("a", "val_a1");
		A.getAttributes().emplace("a", "val_a2");
		A.getAttributes().emplace("b", "val_b");

		auto& B = root.emplaceNested("B");
		B.getAttributes().emplace("a", "val_a1");
		B.getAttributes().emplace("a", "val_a2");
		B.getAttributes().emplace("b", "val_b");

		for (std::size_t k = 0; k < 3; ++k) {
			auto& C = B.emplaceNested("C");
			C.getAttributes().emplace("a", "val_a");
			C.getAttributes().emplace("b", "val_b");
			C.getAttributes().emplace("c", "val_c");
		}

		B.emplaceNested("D");
	}

	std::cout << "The composed xml has this content " << std::endl;
	std::cout << root << std::endl;

	nlohmann::json root_as_json = root;
	std::cout << "Converted as a json it becomes " << std::endl;
	std::cout << root_as_json.dump(1) << std::endl;
}
