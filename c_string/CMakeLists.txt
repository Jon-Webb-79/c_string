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
project(c_string)
set(CMAKE_C_STANDARD 17)
set(CMAKE_C_STANDARD_REQUIRED ON)
enable_testing()

# Set compiler flags
if ("${CMAKE_C_COMPILER_ID}" STREQUAL "GNU" OR "${CMAKE_C_COMPILER_ID}" STREQUAL "Clang")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Werror -Wpedantic")
endif()

# Platform-specific configurations
if(UNIX AND NOT APPLE)
    # Linux-specific settings if needed
    add_definitions(-D_GNU_SOURCE)
elseif(APPLE)
    # macOS-specific settings if needed
    add_definitions(-D_DARWIN_C_SOURCE)
endif()

# Add the library
add_library(c_string
    c_string.c
)

target_include_directories(c_string PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/c_string/include)

# Add the test directory
add_subdirectory(test)

# cmake_minimum_required(VERSION 3.31.3)
# project(c_string)
#
#
# set(CMAKE_C_STANDARD 17)
# set(CMAKE_C_STANDARD_REQUIRED ON)
#
# enable_testing()
#
# # Set compiler flags
# if ("${CMAKE_C_COMPILER_ID}" STREQUAL "GNU" OR "${CMAKE_C_COMPILER_ID}" STREQUAL "Clang")
#     set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Werror -Wpedantic")
# endif()
#
# # Add the `endf` library
# add_library(c_string
#             c_string.c
# )
#
# # Link against jansson
#
# target_include_directories(c_string PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/c_string/include)
#
# # Add the test directory
# add_subdirectory(test)
# ================================================================================
# ================================================================================
# eof
