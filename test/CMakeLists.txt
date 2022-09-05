# Minimum version of CMake required that supports all features used by this file.
cmake_minimum_required(VERSION 3.23)

# The project's name and its semantic version number.
project(dlclass_test VERSION ${dlclass_VERSION})

# Create a library with an example plugin to test.
add_library(dlclass_example SHARED $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>/src/dlclass/test/example.cpp)
target_include_directories(dlclass_example PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>/include)

# Create a program to test the example plugin.
add_executable(dlclass_test $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>/src/dlclass/test.cpp)
target_include_directories(dlclass_test PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>/include)
target_link_libraries(dlclass_test PUBLIC dlclass)

# Create a test.
add_test(NAME dlclass_test COMMAND dlclass_test)
