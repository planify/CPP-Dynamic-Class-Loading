cmake_minimum_required(VERSION 3.23)

project(dlclass VERSION 0.1.0)

set(dlclass_INCLUDE_DIR $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>/include)

macro(add_plugin_library lib_NAME)
  cmake_parse_arguments(lib "" "" "SOURCES" ${ARGN})
  add_library(${PROJECT_NAME}_${lib_NAME} SHARED ${lib_SOURCES})
  target_include_directories(${PROJECT_NAME}_${lib_NAME} PUBLIC ${dlclass_INCLUDE_DIR})
endmacro()

add_plugin_library(square SOURCES src/square.cpp)
add_plugin_library(triangle SOURCES src/triangle.cpp)
add_plugin_library(foo SOURCES src/foo_impl.cpp)

enable_testing()
set(TEST_NAME ${PROJECT_NAME}_test)
add_executable(${TEST_NAME} test/dlclass_test.cpp)
target_include_directories(${TEST_NAME} PUBLIC ${dlclass_INCLUDE_DIR})
add_test(NAME ${TEST_NAME} COMMAND ${PROJECT_NAME}_test)
