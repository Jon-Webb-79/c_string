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
#include <string.h>
#include <limits.h>

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
// --------------------------------------------------------------------------------

// Test string with special characters
void test_init_string_special_chars(void **state) {
    const char* test_str = "Hello\n\t!@#$%^&*()";
    //string_t* str = init_string("Hello");
    string_t* str = init_string(test_str);
    assert_non_null(str);

    assert_string_equal(get_string(str), "Hello\n\t!@#$%^&*()");
    assert_int_equal(string_size(str), 17);
    assert_int_equal(string_alloc(str), 18);
    free_string(str);
}
// --------------------------------------------------------------------------------

// Test very long string
void test_init_string_long(void **state) {
    char long_str[1000];
    memset(long_str, 'a', 999);
    long_str[999] = '\0';
    
    string_t* str = init_string(long_str);
    assert_non_null(str);
    assert_string_equal(get_string(str), long_str);
    assert_int_equal(string_size(str), 999);
    assert_int_equal(string_alloc(str), 1000);
    free_string(str);
}
// --------------------------------------------------------------------------------

// Test getter functions with NULL input
void test_getters_null(void **state) {
    assert_null(get_string(NULL));
    assert_int_equal(string_size(NULL), LONG_MAX);
    assert_int_equal(string_alloc(NULL), LONG_MAX);
    assert_int_equal(errno, EINVAL);
}
// --------------------------------------------------------------------------------

#if defined(__GNUC__) || defined(__clang__)
    void test_string_cleanup_gbc(void **state) {
        string_t* str STRING_GBC = init_string("hello world");
        assert_non_null(str);
        assert_string_equal(get_string(str), "hello world");
        assert_int_equal(string_size(str), 11);
        assert_int_equal(string_alloc(str), 12);  // length + null terminator
    }
#endif
// --------------------------------------------------------------------------------

/* Test cases for string concatenation */
void test_concat_string_nominal(void **state) {
    string_t* str1 = init_string("Hello ");
    string_t* str2 = init_string("World!");
    
    assert_true(string_concat(str1, str2));
    assert_string_equal(get_string(str1), "Hello World!");
    assert_int_equal(string_size(str1), 12);
    assert_int_equal(string_alloc(str1), 13);
    
    free_string(str1);
    free_string(str2);
}
// --------------------------------------------------------------------------------

void test_concat_literal_nominal(void **state) {
    string_t* str = init_string("Hello ");
    
    assert_true(string_concat(str, "World!"));
    assert_string_equal(get_string(str), "Hello World!");
    assert_int_equal(string_size(str), 12);
    assert_int_equal(string_alloc(str), 13);
    
    free_string(str);
}
// --------------------------------------------------------------------------------

void test_concat_empty_string(void **state) {
    string_t* str1 = init_string("Hello");
    string_t* str2 = init_string("");
    
    assert_true(string_concat(str1, str2));
    assert_string_equal(get_string(str1), "Hello");
    assert_int_equal(string_size(str1), 5);
    
    free_string(str1);
    free_string(str2);
}
// --------------------------------------------------------------------------------

void test_concat_empty_literal(void **state) {
    string_t* str = init_string("Hello");
    
    assert_true(string_concat(str, ""));
    assert_string_equal(get_string(str), "Hello");
    assert_int_equal(string_size(str), 5);
    
    free_string(str);
}
// --------------------------------------------------------------------------------

void test_concat_null_inputs(void **state) {
    string_t* str = init_string("Hello");
    
    assert_false(string_concat(NULL, str));
    assert_int_equal(errno, EINVAL);
    
    assert_false(string_concat(str, NULL));
    assert_int_equal(errno, EINVAL);
    
    free_string(str);
}
// --------------------------------------------------------------------------------

void test_concat_reallocation(void **state) {
    string_t* str = init_string("Hello");  // 6 bytes allocated
    assert_true(string_concat(str, " World! This is a longer string to force reallocation"));
    assert_string_equal(get_string(str), "Hello World! This is a longer string to force reallocation");
    
    free_string(str);
}
// --------------------------------------------------------------------------------

void test_concat_multiple_times(void **state) {
    string_t* str = init_string("Hello");
    
    assert_true(string_concat(str, " "));
    assert_true(string_concat(str, "World"));
    assert_true(string_concat(str, "!"));
    
    assert_string_equal(get_string(str), "Hello World!");
    assert_int_equal(string_size(str), 12);
    
    free_string(str);
}
// --------------------------------------------------------------------------------

void test_concat_special_characters(void **state) {
    string_t* str = init_string("Line1");
    assert_true(string_concat(str, "\n\tLine2"));
    assert_string_equal(get_string(str), "Line1\n\tLine2");
    
    free_string(str);
}
// --------------------------------------------------------------------------------

void test_concat_large_strings(void **state) {
    // Create two 1000-char strings
    char large_str[1001] = {0};
    memset(large_str, 'A', 1000);
    
    string_t* str1 = init_string(large_str);
    memset(large_str, 'B', 1000);
    
    assert_true(string_concat(str1, large_str));
    assert_int_equal(string_size(str1), 2000);
    
    // Verify first 1000 are 'A' and next 1000 are 'B'
    const char* result = get_string(str1);
    for(int i = 0; i < 1000; i++)
        assert_int_equal(result[i], 'A');
    for(int i = 1000; i < 2000; i++)
        assert_int_equal(result[i], 'B');
    
    free_string(str1);
}
// --------------------------------------------------------------------------------

/* Test cases for string comparison */
void test_compare_strings_equal(void **state) {
    string_t* str1 = init_string("hello");
    string_t* str2 = init_string("hello");
    
    assert_int_equal(compare_strings(str1, str2), 0);
    assert_int_equal(compare_strings(str1, "hello"), 0);
    
    free_string(str1);
    free_string(str2);
}
// -------------------------------------------------------------------------------- 

void test_compare_strings_less(void **state) {
    string_t* str1 = init_string("hello");
    string_t* str2 = init_string("world");
    
    assert_true(compare_strings(str1, str2) < 0);
    assert_true(compare_strings(str1, "world") < 0);
    
    free_string(str1);
    free_string(str2);
}
// --------------------------------------------------------------------------------

void test_compare_strings_greater(void **state) {
    string_t* str1 = init_string("world");
    string_t* str2 = init_string("hello");
    
    assert_true(compare_strings(str1, str2) > 0);
    assert_true(compare_strings(str1, "hello") > 0);
    
    free_string(str1);
    free_string(str2);
}
// --------------------------------------------------------------------------------

void test_compare_strings_different_lengths(void **state) {
    string_t* str1 = init_string("hello");
    string_t* str2 = init_string("hello world");
    
    assert_true(compare_strings(str1, str2) < 0);
    assert_true(compare_strings(str1, "hello world") < 0);
    
    free_string(str1);
    free_string(str2);
}
// --------------------------------------------------------------------------------

void test_compare_strings_empty(void **state) {
    string_t* str1 = init_string("");
    string_t* str2 = init_string("");
    
    assert_int_equal(compare_strings(str1, str2), 0);
    assert_int_equal(compare_strings(str1, ""), 0);
    
    free_string(str1);
    free_string(str2);
}
// -------------------------------------------------------------------------------- 

void test_compare_strings_null(void **state) {
    string_t* str = init_string("hello");
    
    assert_int_equal(compare_strings(NULL, str), INT_MIN);
    assert_int_equal(compare_strings(str, NULL), INT_MIN);
    assert_int_equal(errno, EINVAL);
    
    free_string(str);
}
// -------------------------------------------------------------------------------- 

void test_compare_strings_case_sensitivity(void **state) {
    string_t* str1 = init_string("Hello");
    string_t* str2 = init_string("hello");
    
    assert_true(compare_strings(str1, str2) < 0);  // 'H' < 'h' in ASCII
    assert_true(compare_strings(str1, "hello") < 0);
    
    free_string(str1);
    free_string(str2);
}
// --------------------------------------------------------------------------------
/* Test cases for copy_string and reserve_string */
void test_copy_string_nominal(void **state) {
    string_t* str1 = init_string("hello world");
    string_t* str2 = copy_string(str1);
   
    assert_non_null(str2);
    assert_string_equal(get_string(str2), get_string(str1));
    assert_int_equal(string_size(str2), string_size(str1));
    assert_int_equal(string_alloc(str2), string_alloc(str1));
   
    free_string(str1);
    free_string(str2);
}
// --------------------------------------------------------------------------------

void test_copy_string_empty(void **state) {
    string_t* str1 = init_string("");
    string_t* str2 = copy_string(str1);
   
    assert_non_null(str2);
    assert_string_equal(get_string(str2), "");
    assert_int_equal(string_size(str2), 0);
    assert_int_equal(string_alloc(str2), 1);
   
    free_string(str1);
    free_string(str2);
}
// --------------------------------------------------------------------------------

void test_copy_string_null(void **state) {
    string_t* str = copy_string(NULL);
    assert_null(str);
    assert_int_equal(errno, EINVAL);
}
// --------------------------------------------------------------------------------

void test_reserve_string_nominal(void **state) {
    string_t* str = init_string("test");
    assert_true(reserve_string(str, 20));
    
    assert_int_equal(string_alloc(str), 20);
    assert_string_equal(get_string(str), "test");  // Verify content unchanged
    assert_int_equal(string_size(str), 4);         // Verify length unchanged
   
    free_string(str);
}
// --------------------------------------------------------------------------------

void test_reserve_string_smaller_size(void **state) {
   string_t* str = init_string("test");
   size_t original_alloc = string_alloc(str);
   
   assert_false(reserve_string(str, 2));  // Should fail when trying to reserve less space
   assert_int_equal(errno, EINVAL);
   assert_int_equal(string_alloc(str), original_alloc);  // Allocation should be unchanged
   
   free_string(str);
}
// --------------------------------------------------------------------------------

void test_reserve_string_null(void **state) {
   assert_false(reserve_string(NULL, 10));
   assert_int_equal(errno, EINVAL);
}
// -------------------------------------------------------------------------------- 

void test_trim_string_nominal(void **state) {
    string_t* str = init_string("test");
    // First reserve extra space
    assert_true(reserve_string(str, 20));
    assert_int_equal(string_alloc(str), 20);
    
    // Now trim it
    assert_true(trim_string(str));
    assert_int_equal(string_size(str), 4);
    assert_int_equal(string_alloc(str), 5);  // length + null terminator
    assert_string_equal(get_string(str), "test");
    
    free_string(str);
}
// -------------------------------------------------------------------------------- 

void test_trim_already_minimal(void **state) {
    string_t* str = init_string("test");
    // Should already be minimal (len + 1)
    assert_true(trim_string(str));  // Should succeed but do nothing
    assert_int_equal(string_size(str), 4);
    assert_int_equal(string_alloc(str), 5);
    assert_string_equal(get_string(str), "test");
    
    free_string(str);
}
// -------------------------------------------------------------------------------- 

void test_trim_empty_string(void **state) {
    string_t* str = init_string("");
    // Reserve some space first
    assert_true(reserve_string(str, 10));
    assert_int_equal(string_alloc(str), 10);
    
    // Now trim it
    assert_true(trim_string(str));
    assert_int_equal(string_size(str), 0);
    assert_int_equal(string_alloc(str), 1);  // Just null terminator
    assert_string_equal(get_string(str), "");
    
    free_string(str);
}
// -------------------------------------------------------------------------------- 

void test_trim_null_string(void **state) {
    assert_false(trim_string(NULL));
    assert_int_equal(errno, EINVAL);
}
// -------------------------------------------------------------------------------- 

/* Test cases for first_char_occurance */
void test_first_char_occurance_nominal(void **state) {
    string_t* str = init_string("hello world");
    
    // Find first 'l'
    char* result = first_char_occurance(str, 'l');
    assert_non_null(result);
    assert_int_equal(*result, 'l');
    assert_int_equal(result - get_string(str), 2);  // 'l' is at index 2
    
    free_string(str);
}
// -------------------------------------------------------------------------------- 

void test_first_char_occurance_not_found(void **state) {
    string_t* str = init_string("hello world");
    
    char* result = first_char_occurance(str, 'z');
    assert_null(result);
    
    free_string(str);
}
// --------------------------------------------------------------------------------

void test_first_char_occurance_empty_string(void **state) {
    string_t* str = init_string("");
    
    char* result = first_char_occurance(str, 'a');
    assert_null(result);
    
    free_string(str);
}
// -------------------------------------------------------------------------------- 

void test_first_char_occurance_null(void **state) {
    char* result = first_char_occurance(NULL, 'a');
    assert_null(result);
    assert_int_equal(errno, EINVAL);
}
// --------------------------------------------------------------------------------

/* Test cases for last_char_occurance */
void test_last_char_occurance_nominal(void **state) {
    string_t* str = init_string("hello world");
    
    // Find last 'l'
    char* result = last_char_occurance(str, 'l');
    assert_non_null(result);
    assert_int_equal(*result, 'l');
    assert_int_equal(result - get_string(str), 9);  // Last 'l' is at index 9
    
    free_string(str);
}
// -------------------------------------------------------------------------------- 

void test_last_char_occurance_first_char(void **state) {
    string_t* str = init_string("hello world");
    
    // Find 'h' which is at the start
    char* result = last_char_occurance(str, 'h');
    assert_non_null(result);
    assert_int_equal(*result, 'h');
    assert_int_equal(result - get_string(str), 0);
    
    free_string(str);
}
// -------------------------------------------------------------------------------- 

void test_last_char_occurance_not_found(void **state) {
    string_t* str = init_string("hello world");
    
    char* result = last_char_occurance(str, 'z');
    assert_null(result);
    
    free_string(str);
}
// --------------------------------------------------------------------------------

void test_last_char_occurance_empty_string(void **state) {
    string_t* str = init_string("");
    
    char* result = last_char_occurance(str, 'a');
    assert_null(result);
    
    free_string(str);
}
// --------------------------------------------------------------------------------

void test_last_char_occurance_null(void **state) {
    char* result = last_char_occurance(NULL, 'a');
    assert_null(result);
    assert_int_equal(errno, EINVAL);
}
// ================================================================================
// ================================================================================
// eof
