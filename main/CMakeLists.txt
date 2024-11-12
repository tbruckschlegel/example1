cmake_minimum_required(VERSION 3.25.0)

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

# Allow relative include paths
include_directories(./ ../)

# OS specific code
if(OS_MACOSX)
  set(PLATFORM_SRCS
    util/win/performance_profiler_win.cpp
  )
elseif(WIN32)
  set(PLATFORM_SRCS
    util/win/performance_profiler_win.cpp
  )
else()
  message(FATAL_ERROR "OS not defined!")
endif()

# Function code 
set(FUNC_SRCS
  )

# Util code 
set(UTIL_SRCS
  util/performance_profiler.hpp
  util/performance_profiler.cpp
  util/perf_macros.h
  util/timer.hpp
  )

# Main entry point
set(MAIN_SRCS
  ${FUNC_SRCS}
  ${UTIL_SRCS}
  ${PLATFORM_SRCS}
  main.cpp
  main.hpp
  )

# Unit tests
set(TESTS_SRCS
  ${FUNC_SRCS}
  ${UTIL_SRCS}
  ${PLATFORM_SRCS}
  tests/main.cpp
  tests/timer_unittest.cpp
  tests/performance_profiler_unittest.cpp
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
target_compile_options(${TESTS_TARGET} PRIVATE "$<$<CONFIG:DEBUG>:/W3>")
target_compile_options(${TESTS_TARGET} PRIVATE "$<$<CONFIG:RELEASE>:/W3>")
target_compile_options(${MAIN_TARGET} PRIVATE "$<$<CONFIG:DEBUG>:/W4>")
target_compile_options(${MAIN_TARGET} PRIVATE "$<$<CONFIG:RELEASE>:/W4>")

# STD C++
set_property(TARGET ${MAIN_TARGET} PROPERTY CXX_STANDARD 20)
set_property(TARGET ${TESTS_TARGET} PROPERTY CXX_STANDARD 20)

