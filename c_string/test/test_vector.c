// ================================================================================
// ================================================================================
// - File:    test_vector.c
// - Purpose: This file contains implementations for functions that test the 
//            string vector with a cmocka test framework
//
// Source Metadata
// - Author:  Jonathan A. Webb
// - Date:    August 31, 2022
// - Version: 0.1
// - Copyright: Copyright 2022, Jon Webb Inc.
// ================================================================================
// ================================================================================
// Include modules here

#include "test_vector.h"

#include "../c_string.h"
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
// --------------------------------------------------------------------------------

void test_insert_str_vector_middle(void **state) {
    string_v* vec = init_str_vector(3);
   
    // Create initial vector: "first", "third"
    push_back_str_vector(vec, "first");
    push_back_str_vector(vec, "third");
   
    // Insert in middle
    assert_true(insert_str_vector(vec, "second", 1));
    assert_int_equal(str_vector_size(vec), 3);
   
    // Verify order
    assert_string_equal(get_string(str_vector_index(vec, 0)), "first");
    assert_string_equal(get_string(str_vector_index(vec, 1)), "second");
    assert_string_equal(get_string(str_vector_index(vec, 2)), "third");
   
    free_str_vector(vec);
}
// --------------------------------------------------------------------------------

void test_insert_str_vector_start(void **state) {
    string_v* vec = init_str_vector(2);
   
    push_back_str_vector(vec, "old_first");
   
    // Insert at start
    assert_true(insert_str_vector(vec, "new_first", 0));
    assert_int_equal(str_vector_size(vec), 2);
   
    assert_string_equal(get_string(str_vector_index(vec, 0)), "new_first");
    assert_string_equal(get_string(str_vector_index(vec, 1)), "old_first");
   
    free_str_vector(vec);
}
// --------------------------------------------------------------------------------

void test_insert_str_vector_end(void **state) {
    string_v* vec = init_str_vector(2);
   
    push_back_str_vector(vec, "first");
   
    // Insert at end (equivalent to push_back)
    assert_true(insert_str_vector(vec, "last", 1));
    assert_int_equal(str_vector_size(vec), 2);
    
    assert_string_equal(get_string(str_vector_index(vec, 0)), "first");
    assert_string_equal(get_string(str_vector_index(vec, 1)), "last");
   
    free_str_vector(vec);
}
// --------------------------------------------------------------------------------

void test_insert_str_vector_reallocation(void **state) {
    string_v* vec = init_str_vector(1);
   
    // Fill initial allocation
    push_back_str_vector(vec, "first");
    size_t initial_alloc = str_vector_alloc(vec);
   
    // This should trigger resize
    assert_true(insert_str_vector(vec, "middle", 1));
    assert_true(str_vector_alloc(vec) > initial_alloc);
   
    assert_string_equal(get_string(str_vector_index(vec, 0)), "first");
    assert_string_equal(get_string(str_vector_index(vec, 1)), "middle");
   
    free_str_vector(vec);
}
// --------------------------------------------------------------------------------

void test_insert_str_vector_invalid_index(void **state) {
    string_v* vec = init_str_vector(2);
    push_back_str_vector(vec, "first");
   
    // Try to insert beyond end
    assert_false(insert_str_vector(vec, "invalid", 2));
    assert_int_equal(errno, ERANGE);
   
    // Verify original string unchanged
    assert_int_equal(str_vector_size(vec), 1);
    assert_string_equal(get_string(str_vector_index(vec, 0)), "first");
   
    free_str_vector(vec);
}
// --------------------------------------------------------------------------------

void test_insert_str_vector_null_inputs(void **state) {
    string_v* vec = init_str_vector(2);
   
    assert_false(insert_str_vector(NULL, "test", 0));
    assert_int_equal(errno, EINVAL);
   
    assert_false(insert_str_vector(vec, NULL, 0));
    assert_int_equal(errno, EINVAL);
   
    free_str_vector(vec);
}
// --------------------------------------------------------------------------------

void test_insert_str_vector_empty_string(void **state) {
    string_v* vec = init_str_vector(2);
    push_back_str_vector(vec, "test");
   
    assert_true(insert_str_vector(vec, "", 1));
    assert_string_equal(get_string(str_vector_index(vec, 1)), "");
    assert_int_equal(string_size(str_vector_index(vec, 1)), 0);
   
    free_str_vector(vec);
}
// --------------------------------------------------------------------------------

void test_pop_back_nominal(void **state) {
    string_v* vec = init_str_vector(2);
    push_back_str_vector(vec, "first");
    push_back_str_vector(vec, "second");
   
    string_t* popped = pop_back_str_vector(vec);
    assert_non_null(popped);
    assert_string_equal(get_string(popped), "second");
    assert_int_equal(str_vector_size(vec), 1);
    assert_string_equal(get_string(str_vector_index(vec, 0)), "first");
   
    free_string(popped);
    free_str_vector(vec);
}
// --------------------------------------------------------------------------------

void test_pop_front_nominal(void **state) {
    string_v* vec = init_str_vector(2);
    push_back_str_vector(vec, "first");
    push_back_str_vector(vec, "second");
   
    string_t* popped = pop_front_str_vector(vec);
    assert_non_null(popped);
    assert_string_equal(get_string(popped), "first");
    assert_int_equal(str_vector_size(vec), 1);
    assert_string_equal(get_string(str_vector_index(vec, 0)), "second");
   
    free_string(popped);
    free_str_vector(vec);
}
// --------------------------------------------------------------------------------

void test_pop_empty_vector(void **state) {
    string_v* vec = init_str_vector(1);
   
    string_t* back = pop_back_str_vector(vec);
    assert_null(back);
    assert_int_equal(errno, EINVAL);
   
    string_t* front = pop_front_str_vector(vec);
    assert_null(front);
    assert_int_equal(errno, EINVAL);
   
    free_str_vector(vec);
}
// --------------------------------------------------------------------------------

void test_pop_multiple_back(void **state) {
    string_v* vec = init_str_vector(3);
    push_back_str_vector(vec, "one");
    push_back_str_vector(vec, "two");
    push_back_str_vector(vec, "three");
   
    // Pop all elements from back
    string_t* third = pop_back_str_vector(vec);
    assert_string_equal(get_string(third), "three");
    free_string(third);
   
    string_t* second = pop_back_str_vector(vec);
    assert_string_equal(get_string(second), "two");
    free_string(second);
   
    string_t* first = pop_back_str_vector(vec);
    assert_string_equal(get_string(first), "one");
    free_string(first);
   
    assert_int_equal(str_vector_size(vec), 0);
   
    free_str_vector(vec);
}
// --------------------------------------------------------------------------------

void test_pop_multiple_front(void **state) {
    string_v* vec = init_str_vector(3);
    push_back_str_vector(vec, "one");
    push_back_str_vector(vec, "two");
    push_back_str_vector(vec, "three");
   
    // Pop all elements from front
    string_t* first = pop_front_str_vector(vec);
    assert_string_equal(get_string(first), "one");
    assert_string_equal(get_string(str_vector_index(vec, 0)), "two");
    free_string(first);
   
    string_t* second = pop_front_str_vector(vec);
    assert_string_equal(get_string(second), "two");
    assert_string_equal(get_string(str_vector_index(vec, 0)), "three");
    free_string(second);
   
    string_t* third = pop_front_str_vector(vec);
    assert_string_equal(get_string(third), "three");
    free_string(third);
   
    assert_int_equal(str_vector_size(vec), 0);
   
    free_str_vector(vec);
}
// --------------------------------------------------------------------------------

void test_pop_null_vector(void **state) {
    assert_null(pop_back_str_vector(NULL));
    assert_int_equal(errno, EINVAL);
   
    assert_null(pop_front_str_vector(NULL));
    assert_int_equal(errno, EINVAL);
}
// --------------------------------------------------------------------------------

void test_pop_alternating(void **state) {
    string_v* vec = init_str_vector(3);
    push_back_str_vector(vec, "one");
    push_back_str_vector(vec, "two");
    push_back_str_vector(vec, "three");
   
    string_t* back = pop_back_str_vector(vec);
    assert_string_equal(get_string(back), "three");
    free_string(back);
   
    string_t* front = pop_front_str_vector(vec);
    assert_string_equal(get_string(front), "one");
    free_string(front);
   
    assert_int_equal(str_vector_size(vec), 1);
    assert_string_equal(get_string(str_vector_index(vec, 0)), "two");
   
    free_str_vector(vec);
}
// --------------------------------------------------------------------------------

/* Tests for pop_any_str_vector */
void test_pop_any_nominal(void **state) {
    string_v* vec = init_str_vector(3);
    push_back_str_vector(vec, "first");
    push_back_str_vector(vec, "second");
    push_back_str_vector(vec, "third");
   
    // Pop middle element
    string_t* popped = pop_any_str_vector(vec, 1);
    assert_non_null(popped);
    assert_string_equal(get_string(popped), "second");
   
    // Verify remaining elements and order
    assert_int_equal(str_vector_size(vec), 2);
    assert_string_equal(get_string(str_vector_index(vec, 0)), "first");
    assert_string_equal(get_string(str_vector_index(vec, 1)), "third");
   
    free_string(popped);
    free_str_vector(vec);
}
// --------------------------------------------------------------------------------

void test_pop_any_first(void **state) {
    string_v* vec = init_str_vector(3);
    push_back_str_vector(vec, "first");
    push_back_str_vector(vec, "second");
    push_back_str_vector(vec, "third");
   
    // Pop first element
    string_t* popped = pop_any_str_vector(vec, 0);
    assert_non_null(popped);
    assert_string_equal(get_string(popped), "first");
   
    // Verify remaining elements shifted correctly
    assert_int_equal(str_vector_size(vec), 2);
    assert_string_equal(get_string(str_vector_index(vec, 0)), "second");
    assert_string_equal(get_string(str_vector_index(vec, 1)), "third");
   
    free_string(popped);
    free_str_vector(vec);
}
// --------------------------------------------------------------------------------

void test_pop_any_last(void **state) {
    string_v* vec = init_str_vector(3);
    push_back_str_vector(vec, "first");
    push_back_str_vector(vec, "second");
    push_back_str_vector(vec, "third");
   
    // Pop last element
    string_t* popped = pop_any_str_vector(vec, 2);
    assert_non_null(popped);
    assert_string_equal(get_string(popped), "third");
   
    // Verify remaining elements unchanged
    assert_int_equal(str_vector_size(vec), 2);
    assert_string_equal(get_string(str_vector_index(vec, 0)), "first");
    assert_string_equal(get_string(str_vector_index(vec, 1)), "second");
   
    free_string(popped);
    free_str_vector(vec);
}
// --------------------------------------------------------------------------------

void test_pop_any_invalid_index(void **state) {
    string_v* vec = init_str_vector(2);
    push_back_str_vector(vec, "first");
    push_back_str_vector(vec, "second");
   
    // Try to pop at invalid index
    string_t* popped = pop_any_str_vector(vec, 2);  // Index == len
    assert_null(popped);
    assert_int_equal(errno, ERANGE);
   
    // Verify vector unchanged
    assert_int_equal(str_vector_size(vec), 2);
    assert_string_equal(get_string(str_vector_index(vec, 0)), "first");
    assert_string_equal(get_string(str_vector_index(vec, 1)), "second");
   
    free_str_vector(vec);
}
// -------------------------------------------------------------------------------- 

void test_pop_any_empty_vector(void **state) {
    string_v* vec = init_str_vector(1);
   
    string_t* popped = pop_any_str_vector(vec, 0);
    assert_null(popped);
    assert_int_equal(errno, EINVAL);
   
    free_str_vector(vec);
}
// -------------------------------------------------------------------------------- 

void test_pop_any_null_vector(void **state) {
    assert_null(pop_any_str_vector(NULL, 0));
    assert_int_equal(errno, EINVAL);
}
// -------------------------------------------------------------------------------- 

void test_pop_any_multiple(void **state) {
    string_v* vec = init_str_vector(4);
    assert_int_equal(4, s_alloc(vec));
    push_back_str_vector(vec, "one");
    push_back_str_vector(vec, "two");
    push_back_str_vector(vec, "three");
    push_back_str_vector(vec, "four");
   
    assert_int_equal(4, s_size(vec));
    // Pop elements in various positions
    string_t* popped1 = pop_any_str_vector(vec, 1);  // Pop "two"
    assert_int_equal(3, s_size(popped1));
    assert_int_equal(4, s_alloc(popped1));
    assert_string_equal(get_string(popped1), "two");
    free_string(popped1);
   
    string_t* popped2 = pop_any_str_vector(vec, 2);  // Pop "four"
    assert_string_equal(get_string(popped2), "four");
    free_string(popped2);
   
    // Verify remaining elements
    assert_int_equal(str_vector_size(vec), 2);
    assert_string_equal(get_string(str_vector_index(vec, 0)), "one");
    assert_string_equal(get_string(str_vector_index(vec, 1)), "three");
   
    free_str_vector(vec);
}
// --------------------------------------------------------------------------------

void test_sort_empty_vector(void **state) {
    string_v* vec = init_str_vector(1);
   
    sort_str_vector(vec, FORWARD);  // Should handle empty vector gracefully
    assert_int_equal(str_vector_size(vec), 0);
   
    free_str_vector(vec);
}
// --------------------------------------------------------------------------------

void test_sort_single_element(void **state) {
    string_v* vec = init_str_vector(1);
    push_back_str_vector(vec, "test");
   
    sort_str_vector(vec, FORWARD);  // Should handle single element gracefully
    assert_int_equal(str_vector_size(vec), 1);
    assert_string_equal(get_string(str_vector_index(vec, 0)), "test");
   
    free_str_vector(vec);
}
// --------------------------------------------------------------------------------

void test_sort_forward_already_sorted(void **state) {
    string_v* vec = init_str_vector(3);
    push_back_str_vector(vec, "apple");
    push_back_str_vector(vec, "banana");
    push_back_str_vector(vec, "cherry");
   
    sort_str_vector(vec, FORWARD);
    assert_string_equal(get_string(str_vector_index(vec, 0)), "apple");
    assert_string_equal(get_string(str_vector_index(vec, 1)), "banana");
    assert_string_equal(get_string(str_vector_index(vec, 2)), "cherry");
   
    free_str_vector(vec);
}
// --------------------------------------------------------------------------------

void test_sort_forward_reverse_sorted(void **state) {
    string_v* vec = init_str_vector(3);
    push_back_str_vector(vec, "cherry");
    push_back_str_vector(vec, "banana");
    push_back_str_vector(vec, "apple");
   
    sort_str_vector(vec, FORWARD);
    assert_string_equal(get_string(str_vector_index(vec, 0)), "apple");
    assert_string_equal(get_string(str_vector_index(vec, 1)), "banana");
    assert_string_equal(get_string(str_vector_index(vec, 2)), "cherry");
   
    free_str_vector(vec);
}
// --------------------------------------------------------------------------------

void test_sort_reverse_already_sorted(void **state) {
   string_v* vec = init_str_vector(3);
   push_back_str_vector(vec, "cherry");
   push_back_str_vector(vec, "banana");
   push_back_str_vector(vec, "apple");
   
   sort_str_vector(vec, REVERSE);
   assert_string_equal(get_string(str_vector_index(vec, 0)), "cherry");
   assert_string_equal(get_string(str_vector_index(vec, 1)), "banana");
   assert_string_equal(get_string(str_vector_index(vec, 2)), "apple");
   
   free_str_vector(vec);
}
// --------------------------------------------------------------------------------

void test_sort_large_vector(void **state) {
    string_v* vec = init_str_vector(100);
    // Add strings in random order
    push_back_str_vector(vec, "zebra");
    push_back_str_vector(vec, "apple");
    push_back_str_vector(vec, "monkey");
    push_back_str_vector(vec, "bear");
    push_back_str_vector(vec, "Mountain Lion");
    push_back_str_vector(vec, "elk");
    // ... add more strings ...
   
    sort_str_vector(vec, FORWARD);
    // Verify order
    assert_true(compare_strings_string(str_vector_index(vec, 0), 
                                       (string_t*)str_vector_index(vec, 1)) <= 0);
    // ... verify more elements ...
   
    free_str_vector(vec);
}
// -------------------------------------------------------------------------------- 

void test_sort_duplicate_elements(void **state) {
    string_v* vec = init_str_vector(4);
    push_back_str_vector(vec, "apple");
    push_back_str_vector(vec, "banana");
    push_back_str_vector(vec, "apple");
    push_back_str_vector(vec, "cherry");
   
    sort_str_vector(vec, FORWARD);
    assert_string_equal(get_string(str_vector_index(vec, 0)), "apple");
    assert_string_equal(get_string(str_vector_index(vec, 1)), "apple");
    assert_string_equal(get_string(str_vector_index(vec, 2)), "banana");
    assert_string_equal(get_string(str_vector_index(vec, 3)), "cherry");
   
    free_str_vector(vec);
}
// --------------------------------------------------------------------------------

void test_sort_with_empty_strings(void **state) {
    string_v* vec = init_str_vector(3);
    push_back_str_vector(vec, "");
    push_back_str_vector(vec, "apple");
    push_back_str_vector(vec, "");
   
    sort_str_vector(vec, FORWARD);
    assert_string_equal(get_string(str_vector_index(vec, 0)), "");
    assert_string_equal(get_string(str_vector_index(vec, 1)), "");
    assert_string_equal(get_string(str_vector_index(vec, 2)), "apple");
   
    free_str_vector(vec);
}
// -------------------------------------------------------------------------------- 

void test_sort_null_vector(void **state) {
    sort_str_vector(NULL, FORWARD);
    assert_int_equal(errno, EINVAL);
}
// --------------------------------------------------------------------------------

void test_delete_back_nominal(void **state) {
    string_v* vec = init_str_vector(2);
    push_back_str_vector(vec, "first");
    push_back_str_vector(vec, "second");
   
    delete_back_str_vector(vec);
    assert_int_equal(str_vector_size(vec), 1);
    assert_string_equal(get_string(str_vector_index(vec, 0)), "first");
   
    free_str_vector(vec);
}
// --------------------------------------------------------------------------------

void test_delete_front_nominal(void **state) {
    string_v* vec = init_str_vector(2);
    push_back_str_vector(vec, "first");
    push_back_str_vector(vec, "second");
   
    delete_front_str_vector(vec);
    assert_int_equal(str_vector_size(vec), 1);
    assert_string_equal(get_string(str_vector_index(vec, 0)), "second");
   
    free_str_vector(vec);
}
// --------------------------------------------------------------------------------

void test_delete_empty_vector(void **state) {
    string_v* vec = init_str_vector(1);
   
    delete_back_str_vector(vec);
    assert_int_equal(errno, EINVAL);
   
    delete_front_str_vector(vec);
    assert_int_equal(errno, EINVAL);
   
    free_str_vector(vec);
}
// --------------------------------------------------------------------------------

void test_delete_multiple_back(void **state) {
    string_v* vec = init_str_vector(3);
    push_back_str_vector(vec, "one");
    push_back_str_vector(vec, "two");
    push_back_str_vector(vec, "three");
   
    // Pop all elements from back
    delete_back_str_vector(vec);
    delete_back_str_vector(vec);
    delete_back_str_vector(vec);
   
    assert_int_equal(str_vector_size(vec), 0);
   
    free_str_vector(vec);
}
// --------------------------------------------------------------------------------

void test_delete_multiple_front(void **state) {
    string_v* vec = init_str_vector(3);
    push_back_str_vector(vec, "one");
    push_back_str_vector(vec, "two");
    push_back_str_vector(vec, "three");
   
    // Pop all elements from front
    delete_front_str_vector(vec);
    assert_string_equal(get_string(str_vector_index(vec, 0)), "two");
   
    delete_front_str_vector(vec);
    assert_string_equal(get_string(str_vector_index(vec, 0)), "three");
   
    delete_front_str_vector(vec);
   
    assert_int_equal(str_vector_size(vec), 0);
   
    free_str_vector(vec);
}
// --------------------------------------------------------------------------------

void test_delete_null_vector(void **state) {
    assert_null(delete_back_str_vector(NULL));
    assert_int_equal(errno, EINVAL);
   
    assert_null(delete_front_str_vector(NULL));
    assert_int_equal(errno, EINVAL);
}
// --------------------------------------------------------------------------------

void test_delete_alternating(void **state) {
    string_v* vec = init_str_vector(3);
    push_back_str_vector(vec, "one");
    push_back_str_vector(vec, "two");
    push_back_str_vector(vec, "three");
   
    delete_back_str_vector(vec);
    delete_front_str_vector(vec);
   
    assert_int_equal(str_vector_size(vec), 1);
    assert_string_equal(get_string(str_vector_index(vec, 0)), "two");
   
    free_str_vector(vec);
}
// --------------------------------------------------------------------------------

/* Tests for delete_any_str_vector */
void test_delete_any_nominal(void **state) {
    string_v* vec = init_str_vector(3);
    push_back_str_vector(vec, "first");
    push_back_str_vector(vec, "second");
    push_back_str_vector(vec, "third");
   
    // Pop middle element
    delete_any_str_vector(vec, 1);
   
    // Verify remaining elements and order
    assert_int_equal(str_vector_size(vec), 2);
    assert_string_equal(get_string(str_vector_index(vec, 0)), "first");
    assert_string_equal(get_string(str_vector_index(vec, 1)), "third");
   
    free_str_vector(vec);
}
// --------------------------------------------------------------------------------

void test_delete_any_first(void **state) {
    string_v* vec = init_str_vector(3);
    push_back_str_vector(vec, "first");
    push_back_str_vector(vec, "second");
    push_back_str_vector(vec, "third");
   
    // Pop first element
    delete_any_str_vector(vec, 0);
   
    // Verify remaining elements shifted correctly
    assert_int_equal(str_vector_size(vec), 2);
    assert_string_equal(get_string(str_vector_index(vec, 0)), "second");
    assert_string_equal(get_string(str_vector_index(vec, 1)), "third");
   
    free_str_vector(vec);
}
// --------------------------------------------------------------------------------

void test_delete_any_last(void **state) {
    string_v* vec = init_str_vector(3);
    push_back_str_vector(vec, "first");
    push_back_str_vector(vec, "second");
    push_back_str_vector(vec, "third");
   
    // Pop last element
    delete_any_str_vector(vec, 2);
   
    // Verify remaining elements unchanged
    assert_int_equal(str_vector_size(vec), 2);
    assert_string_equal(get_string(str_vector_index(vec, 0)), "first");
    assert_string_equal(get_string(str_vector_index(vec, 1)), "second");
   
    free_str_vector(vec);
}
// --------------------------------------------------------------------------------

void test_delete_any_invalid_index(void **state) {
    string_v* vec = init_str_vector(2);
    push_back_str_vector(vec, "first");
    push_back_str_vector(vec, "second");
   
    // Try to delete at invalid index
    delete_any_str_vector(vec, 2);  // Index == len
    assert_int_equal(errno, ERANGE);
   
    // Verify vector unchanged
    assert_int_equal(str_vector_size(vec), 2);
    assert_string_equal(get_string(str_vector_index(vec, 0)), "first");
    assert_string_equal(get_string(str_vector_index(vec, 1)), "second");
   
    free_str_vector(vec);
}
// -------------------------------------------------------------------------------- 

void test_delete_any_empty_vector(void **state) {
    string_v* vec = init_str_vector(1);
   
    delete_any_str_vector(vec, 0);
    assert_int_equal(errno, EINVAL);
   
    free_str_vector(vec);
}
// -------------------------------------------------------------------------------- 

void test_delete_any_null_vector(void **state) {
    assert_null(delete_any_str_vector(NULL, 0));
    assert_int_equal(errno, EINVAL);
}
// -------------------------------------------------------------------------------- 

void test_delete_any_multiple(void **state) {
    string_v* vec = init_str_vector(4);
    assert_int_equal(4, s_alloc(vec));
    push_back_str_vector(vec, "one");
    push_back_str_vector(vec, "two");
    push_back_str_vector(vec, "three");
    push_back_str_vector(vec, "four");
   
    assert_int_equal(4, s_size(vec));
    // Pop elements in various positions
    delete_any_str_vector(vec, 1);  // Pop "two"
   
    delete_any_str_vector(vec, 2);  // Pop "four"
   
    // Verify remaining elements
    assert_int_equal(str_vector_size(vec), 2);
    assert_string_equal(get_string(str_vector_index(vec, 0)), "one");
    assert_string_equal(get_string(str_vector_index(vec, 1)), "three");
   
    free_str_vector(vec);
}
// --------------------------------------------------------------------------------

void test_reverse_str_vector(void **state) {
    string_v* vec = init_str_vector(4);
    assert_int_equal(4, s_alloc(vec));
    push_back_str_vector(vec, "one");
    push_back_str_vector(vec, "two");
    push_back_str_vector(vec, "three");
    push_back_str_vector(vec, "four");    

    reverse_str_vector(vec);

    char* a[4] = {"four", "three", "two", "one"};
    for (size_t i = 0; i < s_size(vec); i++) {
        assert_string_equal(a[i], get_string(str_vector_index(vec, i)));
    }

    free_str_vector(vec);
}
// ================================================================================
// ================================================================================
// eof
