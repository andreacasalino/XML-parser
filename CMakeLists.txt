cmake_minimum_required(VERSION 3.0 FATAL_ERROR)

option(LIB_OPT "Compile shared libraries (ON) or static (OFF)" OFF)

set(CMAKE_CXX_STANDARD 17)

include(${CMAKE_CURRENT_SOURCE_DIR}/cmake/MakeLibrary.cmake)

set(CMAKE_INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/lib")
		
project(XML-Parser-Core)
add_subdirectory(Lib)

option(BUILD_XML_Parser_SAMPLES "Build the samples showing how to use XML-Parser" ON)
if(BUILD_XML_Parser_SAMPLES)
    add_subdirectory(Examples)
endif()

option(BUILD_XML_Parser_TESTS "Enable tests generation" OFF)
if(BUILD_XML_Parser_TESTS)
    include(FetchContent)
    FetchContent_Declare(
    catch2
    GIT_REPOSITORY https://github.com/catchorg/Catch2.git
    GIT_TAG        4ff8b27bb6bed4b8b309e56cd269b4f1fbc24e89
    )
    FetchContent_MakeAvailable(catch2)

    add_subdirectory(Tests)
endif()
