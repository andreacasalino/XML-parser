include(FetchContent)
set(GTEST_VERSION v1.13.0)
message(STATUS "===> fetching gtest for version ${GTEST_VERSION}")
if (WIN32)
	set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
endif (WIN32)
FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG        ${GTEST_VERSION}
)
FetchContent_MakeAvailable(googletest)

set(TEST_NAME Tests)

file(GLOB SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp ${CMAKE_CURRENT_SOURCE_DIR}/*.h)

add_executable(${TEST_NAME} ${SOURCES})

target_include_directories(${TEST_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../src/source)

target_link_libraries(${TEST_NAME} PUBLIC
    gtest
    gtest_main
    XML-Parser
)

target_compile_definitions(${TEST_NAME} PUBLIC
    -D TEST_DATA="${CMAKE_CURRENT_SOURCE_DIR}/test_data"
)

install(TARGETS ${TEST_NAME})
