option(BUILD_EXAMPLE_01 "Build Example_01" ON)
if(BUILD_EXAMPLE_01)
    add_executable(Example_01 ./Ex_01/Main_01.cpp)
    target_link_libraries(Example_01 PUBLIC XMLPrs::XML-Parser)	

    add_custom_command(TARGET Example_01 PRE_BUILD
	    COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_SOURCE_DIR}/Ex_01/XML_example_01.xml ${CMAKE_CURRENT_BINARY_DIR}/XML_example_01.xml)
endif()

option(BUILD_EXAMPLE_02 "Build Example_02" ON)
if(BUILD_EXAMPLE_02)
    add_executable(Example_02 ./Ex_02/Main_02.cpp)
    target_link_libraries(Example_02 PUBLIC XMLPrs::XML-Parser)	

    add_custom_command(TARGET Example_02 PRE_BUILD
	    COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_SOURCE_DIR}/Ex_02/XML_example_02.xml ${CMAKE_CURRENT_BINARY_DIR}/XML_example_02.xml)
endif()

option(BUILD_EXAMPLE_03 "Build Example_03" ON)
if(BUILD_EXAMPLE_03)
    add_executable(Example_03 ./Ex_03/Main_03.cpp)
    target_link_libraries(Example_03 PUBLIC XMLPrs::XML-Parser)
endif()

option(BUILD_EXAMPLE_04 "Build Example_04" ON)
if(BUILD_EXAMPLE_04)
    add_executable(Example_04 ./Ex_04/Main_04.cpp)
    target_link_libraries(Example_04 PUBLIC XMLPrs::XML-Parser)	

    add_custom_command(TARGET Example_04 PRE_BUILD
	    COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_SOURCE_DIR}/Ex_04/XML_example_04a.xml ${CMAKE_CURRENT_BINARY_DIR}/XML_example_04a.xml)

    add_custom_command(TARGET Example_04 PRE_BUILD
	    COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_SOURCE_DIR}/Ex_04/XML_example_04b.xml ${CMAKE_CURRENT_BINARY_DIR}/XML_example_04b.xml)
endif()
