![binaries_compilation](https://github.com/andreacasalino/XML-parser/actions/workflows/runTests.yml/badge.svg)

Light **C++** library for managing **xml** structure. It is possible to **import**, **modify** and **export** **xml** structures. At the same time, xml structure can  be **generated from 0** from code and later exported.

All the relevant code is contained in **./Lib**.
If you have found this package to be useful, please leave a **star**.

## USAGE

To parse an **xml** from a file you have just to call 1 single function:
```cpp
#include <XML-Parser/Parser.h>

// import the structure into a parser
auto root_parsed = xmlPrs::parse_xml(file);
```

The returned result might be:
- an error in case something went wrong (invalid file, invalid xml structure, etc...)
- an handler to the root of the parsed structure
Supposing that the import action had no problems, you can access the root to query or modify it:
```cpp
// access the root content, supposing the parse function suceeded
xmlPrs::Root &root = std::get<xmlPrs::Root>(root_parsed);
```

You can also generate **xml** structure from 0, by building an empty root which is later filled:
```cpp
xmlPrs::Root root("Root_name");
```

Adding a tag can be done with just one function call:
```cpp
xmlPrs::Tag &tag_added = root["tag_name"];
```

and the returned reference can be used to modify the tag:
```cpp
// add some attribute
tag_added.getAttributes().emplace("field_A", "value");
tag_added.getAttributes().emplace("field_b", "value");
// add a nested tag
auto& tag_nested = tag_added["nested_tag_name"];
// add an attribute to the newly nested created tag
tag_nested.getAttributes().emplace("field", "value");
```

Haven't already left a **star**? Do it now! :).

You can also reprint the generated or modified structure into an **std::ostream**, which can be also a file stream:
```cpp
// print the structure into a textual file
std::ofstream stream("temp.xml");
stream << root;
// print in the console
std::cout << root << std::endl;
```

Refer to the samples contained in **./Examples** for further details.

## CONVERSION TO JSON

This package contains also an automatic conversion of xml representations to json. 
More specifically, a **xmlPrs::Root** can be automatically converted into a **nlohmann::json**, i.e. the json representation 
contained in the [**nlohmann**](https://github.com/nlohmann/json) library. 
This can easily done with a single instruction:
```cpp
xmlPrs::Root as_xml("Root");
// fill the xml structure
// ...

// convert to json
nlohmann::json as_json = as_xml;
```

Check also **./Examples/Example-05.cpp**.
This functionality is **NOT** default enabled and is controlled by the **CMake** option **ENABLE_XML_Parser_nlohmann_json_converter**. 
When set to **ON**, the [**nlohmann**](https://github.com/nlohmann/json) library is fetched in order to allow the conversion.

## XML-GUI

[XML-GUI](https://github.com/andreacasalino/XML-GUI) wraps this library as C++ backend to a nice graphical user interactive application handling xml:
![Example using XML-GUI](https://github.com/andreacasalino/XML-GUI/blob/master/Example.png)

## TO BE DONE

Support for skipping comments inside an xml when parsing will be soon added.
