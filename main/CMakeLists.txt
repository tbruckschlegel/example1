cmake_minimum_required(VERSION 3.15.1)

# Only generate Debug and Release configuration types for supported variants
set(CMAKE_CONFIGURATION_TYPES Debug Release)

# Target executable names
set(MAIN_TARGET "main")
set(TESTS_TARGET "tests")

set(VCPKG_TARGET_ARCHITECTURE x64)
set(VCPKG_CRT_LINKAGE static)
set(VCPKG_LIBRARY_LINKAGE static)

# Use folders in the resulting project files
set_property(GLOBAL PROPERTY OS_FOLDERS ON)

# Setup testing
set(GTest_DIR "../third_party/vcpkg/vcpkg_installed/x64-windows-static/share/gtest")
find_package(GTest CONFIG REQUIRED)

set(MAIN_SRCS
  main.cpp
  main.hpp
  )

set(TESTS_SRCS
  tests/main.cpp
  )

# Executable targets
add_executable(${MAIN_TARGET} WIN32 ${MAIN_SRCS})
add_executable(${TESTS_TARGET} WIN32 ${TESTS_SRCS})
set_target_properties(${MAIN_TARGET} PROPERTIES LINK_FLAGS /SUBSYSTEM:CONSOLE)
set_target_properties(${TESTS_TARGET} PROPERTIES LINK_FLAGS /SUBSYSTEM:CONSOLE)

# Warnings break the build
set_target_properties(${MAIN_TARGET} PROPERTIES COMPILE_WARNING_AS_ERROR ON)
set_target_properties(${TESTS_TARGET} PROPERTIES COMPILE_WARNING_AS_ERROR ON)

# Additional libraries
target_link_libraries(${TESTS_TARGET} PRIVATE GTest::gtest GTest::gtest_main)

# Linking directories
#target_link_directories(${MAIN_TARGET} PRIVATE $<$<CONFIG:DEBUG>:${CMAKE_SOURCE_DIR}/build/debug>)
#target_link_directories(${MAIN_TARGET} PRIVATE $<$<CONFIG:RELEASE>:${CMAKE_SOURCE_DIR}/build/release>)

#target_link_directories(${TESTS_TARGET} PRIVATE $<$<CONFIG:DEBUG>:${CMAKE_SOURCE_DIR}/build/debug>)
#target_link_directories(${TESTS_TARGET} PRIVATE $<$<CONFIG:RELEASE>:${CMAKE_SOURCE_DIR}/build/release>)

# Linking to static libs
target_compile_options(${TESTS_TARGET} PRIVATE "$<$<CONFIG:DEBUG>:/MTd>")
target_compile_options(${TESTS_TARGET} PRIVATE "$<$<CONFIG:RELEASE>:/MT>")

target_compile_options(${MAIN_TARGET} PRIVATE "$<$<CONFIG:DEBUG>:/MTd>")
target_compile_options(${MAIN_TARGET} PRIVATE "$<$<CONFIG:RELEASE>:/MT>")

# More warnings
target_compile_options(${TESTS_TARGET} PRIVATE "$<$<CONFIG:DEBUG>:/W4>")
target_compile_options(${TESTS_TARGET} PRIVATE "$<$<CONFIG:RELEASE>:/W4>")
target_compile_options(${MAIN_TARGET} PRIVATE "$<$<CONFIG:DEBUG>:/W4>")
target_compile_options(${MAIN_TARGET} PRIVATE "$<$<CONFIG:RELEASE>:/W4>")
