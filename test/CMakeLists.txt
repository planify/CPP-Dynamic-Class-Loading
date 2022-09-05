# Minimum version of CMake required that supports all features used by this file.
cmake_minimum_required(VERSION 3.23)

# Name for the test executable.
set(TEST_NAME ${PROJECT_NAME}_test)

# Create a library with an example plugin to test.
add_library(example SHARED src/example.cpp)
target_include_directories(example PUBLIC include)

# Create a program to test the example plugin.
add_executable(${TEST_NAME} src/test.cpp)
target_include_directories(${TEST_NAME} PUBLIC include)
target_link_libraries(${TEST_NAME} PUBLIC ${PROJECT_NAME})

# Create a test.
add_test(NAME ${TEST_NAME} COMMAND ${TEST_NAME})
