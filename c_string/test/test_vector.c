// ================================================================================
// ================================================================================
// - File:    test_vector.c
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

#include "test_vector.h"

#include "../include/c_string.h"
#include <errno.h>
// ================================================================================ 
// ================================================================================ 

void test_str_vector_init(void **state) {
   string_v* vec = init_str_vector(5);
   
   assert_non_null(vec);
   assert_int_equal(str_vector_size(vec), 0);
   assert_int_equal(str_vector_alloc(vec), 5);
   
   free_str_vector(vec);
}
// --------------------------------------------------------------------------------

void test_str_vector_push_back(void **state) {
   string_v* vec = init_str_vector(2);
   
   // Add first string
   assert_true(push_back_str_vector(vec, "hello"));
   assert_int_equal(str_vector_size(vec), 1);
   assert_string_equal(get_string(str_vector_index(vec, 0)), "hello");
   assert_int_equal(string_size(str_vector_index(vec, 0)), 5);
   
   // Add second string
   assert_true(push_back_str_vector(vec, "world"));
   assert_int_equal(str_vector_size(vec), 2);
   assert_string_equal(get_string(str_vector_index(vec, 1)), "world");
   assert_int_equal(string_size(str_vector_index(vec, 1)), 5);
   
   free_str_vector(vec);
}
// --------------------------------------------------------------------------------

void test_str_vector_auto_resize(void **state) {
   string_v* vec = init_str_vector(1);
//   size_t initial_alloc = str_vector_size(vec);
   
   // Fill initial allocation
   assert_true(push_back_str_vector(vec, "first"));
   assert_int_equal(str_vector_size(vec), 1);
   assert_int_equal(str_vector_alloc(vec), 1);
   
   // This should trigger resize
   assert_true(push_back_str_vector(vec, "second"));
   assert_int_equal(str_vector_size(vec), 2);
   assert_int_equal(str_vector_alloc(vec), 2);
    
   // Verify both strings are intact
   assert_string_equal(get_string(str_vector_index(vec, 0)), "first");
   assert_string_equal(get_string(str_vector_index(vec, 1)), "second");
   
   free_str_vector(vec);
}
// --------------------------------------------------------------------------------

void test_str_vector_null_inputs(void **state) {
   // Test init with zero size
   string_v* vec = init_str_vector(0);
   assert_non_null(vec);
   free_str_vector(vec);
   
   // Test push_back with NULL vector
   assert_false(push_back_str_vector(NULL, "test"));
   assert_int_equal(errno, EINVAL);
   
   // Test push_back with NULL string
   vec = init_str_vector(1);
   assert_false(push_back_str_vector(vec, NULL));
   assert_int_equal(errno, EINVAL);
   free_str_vector(vec);
   
   // Test free with NULL
   free_str_vector(NULL);
   assert_int_equal(errno, EINVAL);
}
// --------------------------------------------------------------------------------

void test_str_vector_empty_string(void **state) {
   string_v* vec = init_str_vector(1);
   
   assert_true(push_back_str_vector(vec, ""));
   assert_int_equal(str_vector_size(vec), 1);
   assert_string_equal(get_string(str_vector_index(vec, 0)), "");
   assert_int_equal(string_size(str_vector_index(vec, 0)), 0);
   
   free_str_vector(vec);
}
// --------------------------------------------------------------------------------

void test_str_vector_multiple_pushes(void **state) {
   string_v* vec = init_str_vector(2);
   const char* strings[] = {"one", "two", "three", "four", "five"};
   
   for (size_t i = 0; i < 5; i++) {
       assert_true(push_back_str_vector(vec, strings[i]));
       assert_int_equal(str_vector_size(vec), i + 1);
       assert_string_equal(get_string(str_vector_index(vec, i)), strings[i]);
   }
   
   free_str_vector(vec);
}
// --------------------------------------------------------------------------------

void test_str_vector_gbc(void **state) {
   string_v* vec STRVEC_GBC = init_str_vector(2);
   const char* strings[] = {"one", "two", "three", "four", "five"};
   
   for (size_t i = 0; i < 5; i++) {
       assert_true(push_back_str_vector(vec, strings[i]));
       assert_int_equal(str_vector_size(vec), i + 1);
       assert_string_equal(get_string(str_vector_index(vec, i)), strings[i]);
   }
}
// --------------------------------------------------------------------------------

void test_push_front_empty_vector(void **state) {
    string_v* vec = init_str_vector(2);
   
    assert_true(push_front_str_vector(vec, "first"));
    assert_int_equal(str_vector_size(vec), 1);
    assert_string_equal(get_string(str_vector_index(vec, 0)), "first");
   
    free_str_vector(vec);
}
// -------------------------------------------------------------------------------- 

void test_push_front_existing_elements(void **state) {
    string_v* vec = init_str_vector(2);
   
    // Add from back first
    assert_true(push_back_str_vector(vec, "back"));
    assert_true(push_front_str_vector(vec, "front"));
   
    assert_int_equal(str_vector_size(vec), 2);
    assert_string_equal(get_string(str_vector_index(vec, 0)), "front");
    assert_string_equal(get_string(str_vector_index(vec, 1)), "back");
   
    free_str_vector(vec);
}
// -------------------------------------------------------------------------------- 

void test_push_front_reallocation(void **state) {
    string_v* vec = init_str_vector(1);
   
    // Fill initial allocation
    assert_true(push_front_str_vector(vec, "first"));
    assert_int_equal(str_vector_size(vec), 1);
    assert_int_equal(str_vector_alloc(vec), 1);
   
    // This should trigger resize
    assert_true(push_front_str_vector(vec, "second"));
    assert_int_equal(str_vector_size(vec), 2);
    assert_true(str_vector_alloc(vec) > 1);
   
    // Verify order is maintained
    assert_string_equal(get_string(str_vector_index(vec, 0)), "second");
    assert_string_equal(get_string(str_vector_index(vec, 1)), "first");
   
    free_str_vector(vec);
}
// -------------------------------------------------------------------------------- 

void test_push_front_multiple(void **state) {
    string_v* vec = init_str_vector(2);
    const char* strings[] = {"three", "two", "one"};
   
    for (size_t i = 0; i < 3; i++) {
        assert_true(push_front_str_vector(vec, strings[i]));
        assert_int_equal(str_vector_size(vec), i + 1);
        assert_string_equal(get_string(str_vector_index(vec, 0)), strings[i]);
    }
   
    // Verify final order
    assert_string_equal(get_string(str_vector_index(vec, 0)), "one");
    assert_string_equal(get_string(str_vector_index(vec, 1)), "two");
    assert_string_equal(get_string(str_vector_index(vec, 2)), "three");
   
    free_str_vector(vec);
}
// -------------------------------------------------------------------------------- 

void test_push_front_null_inputs(void **state) {
    string_v* vec = init_str_vector(1);
   
    assert_false(push_front_str_vector(NULL, "test"));
    assert_int_equal(errno, EINVAL);
   
    assert_false(push_front_str_vector(vec, NULL));
    assert_int_equal(errno, EINVAL);
   
    free_str_vector(vec);
}
// -------------------------------------------------------------------------------- 

void test_push_front_empty_string(void **state) {
    string_v* vec = init_str_vector(1);
   
    assert_true(push_front_str_vector(vec, ""));
    assert_int_equal(str_vector_size(vec), 1);
    assert_string_equal(get_string(str_vector_index(vec, 0)), "");
    assert_int_equal(string_size(str_vector_index(vec, 0)), 0);
   
    free_str_vector(vec);
}
// ================================================================================
// ================================================================================
// eof
