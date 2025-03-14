// ================================================================================
// ================================================================================
// - File:    test_vector.h
// - Purpose: This file contains prototypes for test functions of a string 
//            vector
//
// Source Metadata
// - Author:  Jonathan A. Webb
// - Date:    August 31, 2022
// - Version: 0.1
// - Copyright: Copyright 2022, Jon Webb Inc.
// ================================================================================
// ================================================================================
// Include modules here

#ifndef test_vector_H
#define test_vector_H

#include <stdio.h>
#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <stdint.h>
#include <cmocka.h>
// ================================================================================
// ================================================================================ 

void test_str_vector_init(void **state);
// --------------------------------------------------------------------------------

void test_str_vector_push_back(void **state);
// --------------------------------------------------------------------------------

void test_str_vector_auto_resize(void **state);
// --------------------------------------------------------------------------------

void test_str_vector_null_inputs(void **state);
// --------------------------------------------------------------------------------

void test_str_vector_empty_string(void **state);
// --------------------------------------------------------------------------------

void test_str_vector_multiple_pushes(void **state);
// --------------------------------------------------------------------------------

void test_str_vector_gbc(void **state);
// --------------------------------------------------------------------------------

void test_push_front_empty_vector(void **state);
// -------------------------------------------------------------------------------- 

void test_push_front_existing_elements(void **state);
// -------------------------------------------------------------------------------- 

void test_push_front_reallocation(void **state);
// -------------------------------------------------------------------------------- 

void test_push_front_multiple(void **state);
// -------------------------------------------------------------------------------- 

void test_push_front_null_inputs(void **state);
// -------------------------------------------------------------------------------- 

void test_push_front_empty_string(void **state);
// --------------------------------------------------------------------------------

void test_insert_str_vector_middle(void **state);
// --------------------------------------------------------------------------------

void test_insert_str_vector_start(void **state);
// --------------------------------------------------------------------------------

void test_insert_str_vector_end(void **state);
// --------------------------------------------------------------------------------

void test_insert_str_vector_reallocation(void **state);
// --------------------------------------------------------------------------------

void test_insert_str_vector_invalid_index(void **state);
// --------------------------------------------------------------------------------

void test_insert_str_vector_null_inputs(void **state);
// --------------------------------------------------------------------------------

void test_insert_str_vector_empty_string(void **state);
// --------------------------------------------------------------------------------

void test_pop_back_nominal(void **state);
// --------------------------------------------------------------------------------

void test_pop_front_nominal(void **state);
// --------------------------------------------------------------------------------

void test_pop_empty_vector(void **state);
// --------------------------------------------------------------------------------

void test_pop_multiple_back(void **state);
// --------------------------------------------------------------------------------

void test_pop_multiple_front(void **state);
// --------------------------------------------------------------------------------

void test_pop_null_vector(void **state);
// --------------------------------------------------------------------------------

void test_pop_alternating(void **state);
// --------------------------------------------------------------------------------

void test_pop_any_nominal(void **state);
// --------------------------------------------------------------------------------

void test_pop_any_first(void **state);
// --------------------------------------------------------------------------------

void test_pop_any_last(void **state);
// --------------------------------------------------------------------------------

void test_pop_any_invalid_index(void **state);
// -------------------------------------------------------------------------------- 

void test_pop_any_empty_vector(void **state);
// -------------------------------------------------------------------------------- 

void test_pop_any_null_vector(void **state);
// -------------------------------------------------------------------------------- 

void test_pop_any_multiple(void **state);
// --------------------------------------------------------------------------------

void test_sort_empty_vector(void **state);
// --------------------------------------------------------------------------------

void test_sort_single_element(void **state);
// --------------------------------------------------------------------------------

void test_sort_forward_already_sorted(void **state);
// --------------------------------------------------------------------------------

void test_sort_forward_reverse_sorted(void **state);
// --------------------------------------------------------------------------------

void test_sort_reverse_already_sorted(void **state);
// --------------------------------------------------------------------------------

void test_sort_large_vector(void **state);
// -------------------------------------------------------------------------------- 

void test_sort_duplicate_elements(void **state);
// --------------------------------------------------------------------------------

void test_sort_with_empty_strings(void **state);
// -------------------------------------------------------------------------------- 

void test_sort_null_vector(void **state);
// --------------------------------------------------------------------------------

void test_delete_back_nominal(void **state);
// --------------------------------------------------------------------------------

void test_delete_front_nominal(void **state);
// --------------------------------------------------------------------------------

void test_delete_empty_vector(void **state);
// --------------------------------------------------------------------------------

void test_delete_multiple_back(void **state);
// --------------------------------------------------------------------------------

void test_delete_multiple_front(void **state);
// --------------------------------------------------------------------------------

void test_delete_null_vector(void **state);
// --------------------------------------------------------------------------------

void test_delete_alternating(void **state);
// --------------------------------------------------------------------------------

void test_delete_any_nominal(void **state);
// --------------------------------------------------------------------------------

void test_delete_any_first(void **state);
// --------------------------------------------------------------------------------

void test_delete_any_last(void **state);
// --------------------------------------------------------------------------------

void test_delete_any_invalid_index(void **state);
// -------------------------------------------------------------------------------- 

void test_delete_any_empty_vector(void **state);
// -------------------------------------------------------------------------------- 

void test_delete_any_null_vector(void **state);
// -------------------------------------------------------------------------------- 

void test_delete_any_multiple(void **state);
// --------------------------------------------------------------------------------

void test_reverse_str_vector(void **state);
// ================================================================================
// ================================================================================ 
#endif /* test_vector_H */
// ================================================================================
// ================================================================================
// eof
