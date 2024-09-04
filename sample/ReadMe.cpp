// This file was used to create the README.md and should not be intended as a proper example

#include <XML-Parser/Tag.h>

#include <XML-Parser/Parser.h>

#include <filesystem>
#include <iostream>
#include <fstream>

int main() {
{
    std::filesystem::path path = "aa/bb/cc";
    auto [metaData, xml_root] = xmlPrs::parseFile(path);

    // extract the version from the metadata ... if provided
    if(auto it_version = metaData.find("version"); it_version != metaData.end()) {
        const std::string& version = it_version->second;
        std::cout << "version of the doc: " << version << std::endl;
    }

    std::string root_name = xml_root->getName();

    // get the attributes of the root
    const std::unordered_multimap<std::string, std::string>& root_attributes = xml_root->getAttributes();

    // access any tag at a specified path
    std::filesystem::path nested_path = "root/childA/childB";
    xmlPrs::Tag& nested = xml_root->getDescendant(nested_path);

    // access the attributes of the nested tag
    const std::unordered_multimap<std::string, std::string>& nested_attributes = xml_root->getAttributes();

    // get a subnested tag
    std::filesystem::path sub_nested_path = "childC/childD";
    xmlPrs::Tag& sub_nested = nested.getDescendant(sub_nested_path);
}

{
    xmlPrs::Tag root{"Root"};
    
    // add some attributes
    root.getAttributes().emplace("fieldA", "valA");
    root.getAttributes().emplace("fieldB", "valB");

    // add a nested tag
    xmlPrs::Tag& nested = root.emplaceNested("Child");

    // add attributes to the nested child
    nested.getAttributes().emplace("fieldC", "valC");

    // print on a file
    std::ofstream{"aa/bb/cc/dd"} << root;

    // ... or convert to a string
    std::string as_string = xmlPrs::toString(root);
}

    return 0;
}
