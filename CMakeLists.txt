cmake_minimum_required(VERSION 3.0 FATAL_ERROR)

option(LIB_OPT "Compile shared libraries (ON) or static (OFF)" OFF)

set(CMAKE_CXX_STANDARD 17)

include(${CMAKE_CURRENT_SOURCE_DIR}/cmake/MakeLibrary.cmake)

set(CMAKE_INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/lib")
		
project(XML-Parser)
add_subdirectory(src)

option(BUILD_XML_Parser_SAMPLES "Build the samples showing how to use XML-Parser" ON)
if(BUILD_XML_Parser_SAMPLES)
    add_subdirectory(sample)
endif()

option(BUILD_XML_Parser_TESTS "Enable tests generation" OFF)
if(BUILD_XML_Parser_TESTS)
    add_subdirectory(test)
endif()
