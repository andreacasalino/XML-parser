set(PROJECT_SHORTNAME "XML-Parser")

MakeLibrary(${PROJECT_SHORTNAME} include)

option(ENABLE_XML_Parser_nlohmann_json_converter "Compile functionalities for converting an xml representation into a nlohmann json one" ON)
if(ENABLE_XML_Parser_nlohmann_json_converter)
	target_compile_definitions(${PROJECT_SHORTNAME} PUBLIC 
		ENABLE_XML_Parser_nlohmann_json_converter
	)

	include(FetchContent)
	FetchContent_Declare(
	json_lib
	GIT_REPOSITORY https://github.com/nlohmann/json.git
	GIT_TAG        v3.10.5
	)
	FetchContent_MakeAvailable(json_lib)
	
	target_link_libraries(${PROJECT_SHORTNAME} PUBLIC 
		nlohmann_json::nlohmann_json
	)
endif()
