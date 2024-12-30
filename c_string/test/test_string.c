// ================================================================================
// ================================================================================
// - File:    test.c
// - Purpose: Describe the file purpose here
//
// Source Metadata
// - Author:  Jonathan A. Webb
// - Date:    August 31, 2022
// - Version: 1.0
// - Copyright: Copyright 2022, Jon Webb Inc.
// ================================================================================
// ================================================================================
// Include modules here

#include <errno.h>

#include "test_string.h"
#include "../include/c_string.h"
// ================================================================================
// ================================================================================ 

/* Test cases for init_string */
void test_init_string_nominal(void **state) {
    string_t* str = init_string("hello world");
    assert_non_null(str);
    assert_string_equal(get_string(str), "hello world");
    assert_int_equal(string_size(str), 11);
    assert_int_equal(string_alloc(str), 12);  // length + null terminator
    free_string(str);
}
// --------------------------------------------------------------------------------

void test_init_string_empty(void **state) {
    string_t* str = init_string("");
    assert_non_null(str);
    assert_string_equal(get_string(str), "");
    assert_int_equal(string_size(str), 0);
    assert_int_equal(string_alloc(str), 1);  // just null terminator
    free_string(str);
}
// --------------------------------------------------------------------------------

void test_init_string_null(void **state) {
    string_t* str = init_string(NULL);
    assert_null(str);
    assert_int_equal(errno, EINVAL);
}
// --------------------------------------------------------------------------------

/* Test cases for free_string */
void test_free_string_nominal(void **state) {
    string_t* str = init_string("test string");
    assert_non_null(str);
    free_string(str);
    // Note: Can't test str after free as it's invalid memory
}
// --------------------------------------------------------------------------------

void test_free_string_null(void **state) {
    free_string(NULL);
    assert_int_equal(errno, EINVAL);
}
// ================================================================================
// ================================================================================
// eof
