#include <XML-Parser/Converter.h>
#include <iostream>
using namespace std;

int main() {
	xmlPrs::Root root("Root");
	
	{
		auto& A = root.addNested("A");
		A.getAttributes().emplace("a", "val_a1");
		A.getAttributes().emplace("a", "val_a2");
		A.getAttributes().emplace("b", "val_b");

		//auto& B = root.addNested("B");
		//B.getAttributes().emplace("a", "val_a1");
		//B.getAttributes().emplace("a", "val_a2");
		//B.getAttributes().emplace("b", "val_b");

		//for (std::size_t k = 0; k < 3; ++k) {
		//	auto& C = root.addNested("C");
		//	C.getAttributes().emplace("a", "val_a1");
		//	C.getAttributes().emplace("a", "val_a2");
		//	C.getAttributes().emplace("b", "val_b");
		//}
	}

	auto root_as_json = xmlPrs::to_json(root);

	std::cout << root_as_json.dump() << std::endl;
}
