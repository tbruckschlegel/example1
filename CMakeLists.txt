cmake_minimum_required(VERSION 3.15.1)

# Only generate Debug and Release configuration types for supported variants.
set(CMAKE_CONFIGURATION_TYPES Debug Release)

# Set vcpkg info.
#set(VCPKG_TARGET_TRIPLET x64-windows-mixed)

# Project name.
project(example1)

# Use folders in the resulting project files.
set_property(GLOBAL PROPERTY OS_FOLDERS ON)

# Allow includes relative to the current source directory.
include_directories(
  ${CMAKE_CURRENT_SOURCE_DIR}
  "${CMAKE_CURRENT_SOURCE_DIR}/third_party/vcpkg/vcpkg_installed/x64-windows-static/include"
)


# Include main
message(STATUS "Add main")
add_subdirectory(main)


