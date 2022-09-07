# Minimum version of CMake required that has all the features used by this file.
cmake_minimum_required(VERSION 3.23)

# Adds the configure_package_config_file() and write_basic_package_version_file() commands.
include(CMakePackageConfigHelpers)

# Provides install directory variables as defined by the GNU Coding Standards.
include(GNUInstallDirs)

set(PROJECT_TARGETS ${PROJECT_NAME}-targets)
set(PROJECT_TARGETS_FILE ${PROJECT_TARGETS}.cmake)

set(PROJECT_CONFIG_FILE ${PROJECT_NAME}-config.cmake)
set(PROJECT_CONFIG_VERSION_FILE ${PROJECT_NAME}-config-version.cmake)

set(PROJECT_INSTALL_SUBDIR ${PROJECT_NAME})
set(PROJECT_INSTALL_CMAKEDIR ${CMAKE_INSTALL_DATAROOTDIR}/${PROJECT_INSTALL_SUBDIR}/cmake)

# Add the install include directory to the library.
target_include_directories(${PROJECT_NAME} INTERFACE
  $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>/${PROJECT_INSTALL_SUBDIR})

# Define the export target.
install(TARGETS ${PROJECT_NAME} EXPORT ${PROJECT_TARGETS})

# Create the project's CMake version file.
write_basic_package_version_file(${PROJECT_CONFIG_VERSION_FILE}
  VERSION
  ${PROJECT_VERSION}

  COMPATIBILITY
  SameMajorVersion)

# Generate the CMake build configuration file.
configure_package_config_file(
  ${PROJECT_SOURCE_DIR}/pkg/${PROJECT_CONFIG_FILE}.in
  ${PROJECT_BINARY_DIR}/${PROJECT_CONFIG_FILE}

  INSTALL_DESTINATION
  ${PROJECT_INSTALL_CMAKEDIR})

# Generate the *-targets.cmake file
install(EXPORT ${PROJECT_TARGETS}
  FILE
  ${PROJECT_TARGETS_FILE}

  NAMESPACE
  ${PROJECT_NAMESPACE}

  DESTINATION
  ${PROJECT_INSTALL_CMAKEDIR})

# Install the config and config-version files.
install(
  FILES
  ${PROJECT_BINARY_DIR}/${PROJECT_CONFIG_FILE}
  ${PROJECT_BINARY_DIR}/${PROJECT_CONFIG_VERSION_FILE}

  DESTINATION
  ${PROJECT_INSTALL_CMAKEDIR})

# Install the headers.
install(DIRECTORY ${PROJECT_SOURCE_DIR}/include/${PROJECT_NAME} DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
install(FILES ${PROJECT_SOURCE_DIR}/include/${PROJECT_NAME}.h DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
