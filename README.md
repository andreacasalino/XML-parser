![binaries_compilation](https://github.com/andreacasalino/XML-parser/actions/workflows/runTests.yml/badge.svg)

- [What is this library about](#intro)
- [Usage](#usage)
- [Samples](#samples)
- [CMake support](#cmake-support)
- [Conversion to JSON](#conversion-to-json)
- [XML GUI](#xml-gui)

## INTRO

This is a light modern **C++** library for managing **xml**. It is possible to **import**, **modify** and **export** any **xml**. At the same time, **xml** can  be **generated from 0** in a programmatic way and later exported over a file or a string.

All the relevant code is contained under the [src](./src) folder.
If you have found this package to be useful, please leave a **star**!

## USAGE

To parse an **xml** from a file you have just to call one single function:
```cpp
#include <XML-Parser/Parser.h>

std::filesystem::path path = "aa/bb/cc";
auto [metaData, xml_root] = xmlPrs::parseFile(path);
```

Meta data can be inspected to check the version for example. Notice that the meta data specification is optional for an **xml** and if not provided the meta data collection will be simply empty:
```cpp
// extract the version from the metadata ... if provided
if(auto it_version = metaData.find("version"); it_version != metaData.end()) {
    const std::string& version = it_version->second;
    std::cout << "version of the doc: " << version << std::endl;
}
```

The parsed **xml** can be inspected in many ways:
```cpp
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
```

You can also generate **xml** structure from scratches, by building an empty root which is later filled (or add something to an already parsed **xml**):
```cpp
#include <XML-Parser/Tag.h>

xmlPrs::Tag root{"Root"};

// add some attributes
root.getAttributes().emplace("fieldA", "valA");
root.getAttributes().emplace("fieldB", "valB");

// add a nested tag
xmlPrs::Tag& nested = root.emplaceNested("Child");

// add attributes to the nested child
nested.getAttributes().emplace("fieldC", "valC");
```

You can also reprint the generated or modified structure or convert it to a string:
```cpp
// print on a file
std::ofstream{"aa/bb/cc/dd"} << root;

// ... or convert to a string
std::string as_string = xmlPrs::toString(root);
```

## SAMPLES

Haven't already left a **star**? Do it now! :)!

Under the [sample](./sample) folder you can find more extensive examples showing you how to use this library.

## CMAKE SUPPORT

Haven't already left a **star**? Do it now! :)!

In order to consume this library you can rely on **CMake**. This is the way you could integrate this library in your next **CMake** project:
```cmake
include(FetchContent)
set(BUILD_XML_Parser_SAMPLES OFF CACHE BOOL "" FORCE) # you don't want the samples ... just the library
FetchContent_Declare(
xml_parser
GIT_REPOSITORY https://github.com/andreacasalino/Minimal-Socket
GIT_TAG        master # or a specific release tag if you want ...
)
FetchContent_MakeAvailable(xml_parser)
```
Then, you can link to this library in order to use it:
```cmake
target_link_libraries(THE_TARGET_THAT_NEEDS_TO_CONSUME_XML_PARSER 
PUBLIC 
XML-Parser
)
```

## CONVERSION TO JSON

This package contains also the functionality for automatically converting an **xml** into a **json**. 
More specifically, a **xmlPrs::Tag** can be automatically converted into a **nlohmann::json** (indeed, the well known [nlohmann josn library](https://github.com/nlohmann/json) is used for the **json** representations):
```cpp
xmlPrs::Root as_xml("Root");
// fill the xml structure
// ...

// convert to json
nlohmann::json as_json = as_xml;
```

Check also [Example-05](./sample/Example-05.cpp). In that example, this **xml**:
```xml
<Root>
  <B b="val_b" a="val_a1" a="val_a2">
    <D></D>
    <C c="val_c" b="val_b" a="val_a"></C>
    <C c="val_c" b="val_b" a="val_a"></C>
    <C c="val_c" b="val_b" a="val_a"></C>
  </B>
  <A b="val_b" a="val_a1" a="val_a2"></A>
</Root>
```

is converted to this **json**:
```json
{
 "attributes": null,
 "name": "Root",
 "nested": {
  "A": {
   "attributes": {
    "a": [
     "val_a1",
     "val_a2"
    ],
    "b": "val_b"
   },
   "name": "A",
   "nested": null
  },
  "B": {
   "attributes": {
    "a": [
     "val_a1",
     "val_a2"
    ],
    "b": "val_b"
   },
   "name": "B",
   "nested": {
    "C": [
     {
      "attributes": {
       "a": "val_a",
       "b": "val_b",
       "c": "val_c"
      },
      "name": "C",
      "nested": null
     },
     {
      "attributes": {
       "a": "val_a",
       "b": "val_b",
       "c": "val_c"
      },
      "name": "C",
      "nested": null
     },
     {
      "attributes": {
       "a": "val_a",
       "b": "val_b",
       "c": "val_c"
      },
      "name": "C",
      "nested": null
     }
    ],
    "D": {
     "attributes": null,
     "name": "D",
     "nested": null
    }
   }
  }
 }
}
```

This functionality is by default enabled and is controlled by the **CMake** option **ENABLE_XML_Parser_nlohmann_json_converter**. 
When set to **ON** (default behaviour), the [**nlohmann**](https://github.com/nlohmann/json) library is fetched in order to allow the conversion. On the contrary case, no external dependency is fetched and the functionality to convert into **json** is not provided.

## XML-GUI

Haven't already left a **star**? Do it now! :)!

[XML-GUI](https://github.com/andreacasalino/XML-GUI) wraps this library as C++ backend to a nice graphical user interactive application handling xml:
![Example using XML-GUI](https://github.com/andreacasalino/XML-GUI/blob/master/Example.png)

