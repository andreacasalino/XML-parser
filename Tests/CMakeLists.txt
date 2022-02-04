project(XML-Parser-Tests)

set(TEST_NAME Tests)

file(GLOB SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)

add_executable(${TEST_NAME} ${SOURCES})

target_link_libraries(${TEST_NAME} PUBLIC
    Catch2::Catch2
    Catch2::Catch2WithMain
    XML-Parser
)

target_compile_definitions(${TEST_NAME} PUBLIC
    -D TEST_FOLDER="${CMAKE_CURRENT_SOURCE_DIR}/"
)

install(TARGETS ${TEST_NAME})
