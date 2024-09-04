set(LIB_NAME "XML-Parser")

MakeLibrary(${LIB_NAME} include)

option(BUILD_XML_Parser_COMPILER_CHECK "Enable extra compiler checks" OFF)
if(BUILD_XML_Parser_COMPILER_CHECK)
	target_compile_options(${LIB_NAME} PRIVATE -Wall -Wextra -Wpedantic -Werror)
endif()

target_include_directories(${LIB_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/source)

option(ENABLE_XML_Parser_nlohmann_json_converter "Compile functionalities for converting an xml representation into a nlohmann json one" ON)
if(ENABLE_XML_Parser_nlohmann_json_converter)
	target_compile_definitions(${LIB_NAME} PUBLIC 
		ENABLE_XML_Parser_nlohmann_json_converter
	)


    if (NOT TARGET nlohmann_json::nlohmann_json)
	include(FetchContent)
	FetchContent_Declare(
	json_lib
	GIT_REPOSITORY https://github.com/nlohmann/json.git
	GIT_TAG        v3.10.5
	)
	FetchContent_MakeAvailable(json_lib)
    endif()
	
	target_link_libraries(${LIB_NAME} PUBLIC 
		nlohmann_json::nlohmann_json
	)
endif()
