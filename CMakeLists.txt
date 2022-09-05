# Minimum version of CMake required that supports all features used by this file.
cmake_minimum_required(VERSION 3.23)

# The project's name and its semantic version number.
project(dlclass VERSION 0.1.0)

# The project's CMake options.
option(dlclass_INSTALL "Include install rules" ON)
option(dlclass_TEST "Build test programs" ON)

# Header-only library.
add_library(dlclass INTERFACE)

# The library's installation alias.
add_library(DLClass::DLClass ALIAS dlclass)

# The library depends on these features.
target_compile_features(dlclass INTERFACE cxx_std_20)

# Directory with the header file.
target_include_directories(dlclass INTERFACE $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>/include)

# Optionally enable testing and build the test programs.
if(dlclass_TEST)
  enable_testing()
  add_subdirectory(test)
endif()

# Optionally include the install rules.
if(dlclass_INSTALL)
  include(pkg/dlclass_install.cmake)
endif()
