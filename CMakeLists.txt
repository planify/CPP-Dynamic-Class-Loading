# Minimum version of CMake required that supports all features used by this file.
cmake_minimum_required(VERSION 3.23)

# The project's name and its semantic version number.
project(dlclass VERSION 0.1.0)

set(PROJECT_ALIAS DLClass)
set(PROJECT_NAMESPACE DLClass::)

# The project's CMake options.
option(${PROJECT_NAME}_INSTALL "Include install rules" ON)
option(${PROJECT_NAME}_TEST "Build test programs" ON)

# Header-only library.
add_library(${PROJECT_NAME} INTERFACE)

# The library's installation alias.
add_library(${PROJECT_NAMESPACE}::${PROJECT_AlIAS} ALIAS ${PROJECT_NAME})

# The library depends on these features.
target_compile_features(${PROJECT_NAME} INTERFACE cxx_std_20)

# Directory with the header file.
target_include_directories(${PROJECT_NAME} INTERFACE $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>/include)

# Optionally enable testing and build the test programs.
if(${${PROJECT_NAME}_TEST})
  enable_testing()
  add_subdirectory(test)
endif()

# Optionally include the install rules.
if(${${PROJECT_NAME}_INSTALL})
  include(pkg/${PROJECT_NAME}_install.cmake)
endif()
