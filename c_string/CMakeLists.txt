# ================================================================================
# ================================================================================
# - File:    CMakeLists.txt
# - Purpose: Primary CMake file for C build system
#
# Source Metadata
# - Author:  Jonathan A. Webb
# - Date:    December 30, 2024
# - Version: 1.0
# - Copyright: Copyright 2024, Jonathan A. Webb Inc.
# ================================================================================
# ================================================================================
cmake_minimum_required(VERSION 3.31.3)
project(cstring)

# Set the compiler (Optional)
# set(CMAKE_C_COMPILER "/usr/bin/gcc")

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

enable_testing()

# Set compiler flags
if ("${CMAKE_C_COMPILER_ID}" STREQUAL "GNU" OR "${CMAKE_C_COMPILER_ID}" STREQUAL "Clang")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Werror -Wpedantic")
endif()

# Define the library
add_library(cstring
	hello.c)
target_include_directories(Hello PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

# Define the tests
add_executable(unit_tests
	test/unit_test.c
	test/test_hello.c)
target_link_libraries(unit_tests cstring cmocka)
add_test(NAME unit_tests COMMAND unit_test)

# ================================================================================
# ================================================================================
# eof
