// ================================================================================
// ================================================================================
// - File:    test_vector.h
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
// ================================================================================
// ================================================================================ 
#endif /* test_vector_H */
// ================================================================================
// ================================================================================
// eof
