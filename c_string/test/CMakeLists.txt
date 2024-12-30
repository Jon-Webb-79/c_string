# ================================================================================
# ================================================================================
# - File:    CMakeLists.txt
# - Purpose: Primary CMake file for C++ build system
#
# Source Metadata
# - Author:  Jonathan A. Webb 
# - Date:    December 30, 2024
# - Version: 1.0
# - Copyright: Copyright 2024, Jonathan A. Webb Inc.
# ================================================================================
# ================================================================================
# Set minimum cmake version
# Create the test executable using unit_test.c and test_hello.c
add_executable(unit_tests
	unit_test.c
	test_hello.c)

# Link the test executable against the Hello library and cmocka
target_link_libraries(unit_tests cstring cmocka)

# Register the unit_tests executable as a test for CTest
add_test(NAME unit_tests COMMAND unit_test)

# ================================================================================
# ================================================================================
# eof
