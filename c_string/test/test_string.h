// ================================================================================
// ================================================================================
// - File:    test.h
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

#ifndef test_string_H
#define test_string_H

#include <stdio.h>
#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <stdint.h>
#include <cmocka.h>
// ================================================================================
// ================================================================================

void test_init_string_nominal(void **state);
// --------------------------------------------------------------------------------

void test_init_string_empty(void **state);
// --------------------------------------------------------------------------------

void test_init_string_null(void **state);
// --------------------------------------------------------------------------------

void test_free_string_nominal(void **state);
// --------------------------------------------------------------------------------

void test_free_string_null(void **state);
// --------------------------------------------------------------------------------

void test_init_string_special_chars(void **state);
// --------------------------------------------------------------------------------

void test_init_string_long(void **state);
// --------------------------------------------------------------------------------

void test_getters_null(void **state);
// -------------------------------------------------------------------------------- 

#if defined(__GNUC__) || defined(__clang__)
    void test_string_cleanup_gbc(void **state);
#endif
// -------------------------------------------------------------------------------- 

/* Test cases for string concatenation */
void test_concat_string_nominal(void **state);
// --------------------------------------------------------------------------------

void test_concat_literal_nominal(void **state);
// --------------------------------------------------------------------------------

void test_concat_empty_string(void **state);
// --------------------------------------------------------------------------------

void test_concat_empty_literal(void **state);
// --------------------------------------------------------------------------------

void test_concat_null_inputs(void **state);
// --------------------------------------------------------------------------------

void test_concat_reallocation(void **state);
// --------------------------------------------------------------------------------

void test_concat_multiple_times(void **state);
// --------------------------------------------------------------------------------

void test_concat_special_characters(void **state);
// --------------------------------------------------------------------------------

void test_concat_large_strings(void **state);
// -------------------------------------------------------------------------------- 

void test_compare_strings_equal(void **state);
// -------------------------------------------------------------------------------- 

void test_compare_strings_less(void **state);
// --------------------------------------------------------------------------------

void test_compare_strings_greater(void **state);
// --------------------------------------------------------------------------------

void test_compare_strings_different_lengths(void **state);
// --------------------------------------------------------------------------------

void test_compare_strings_empty(void **state);
// -------------------------------------------------------------------------------- 

void test_compare_strings_null(void **state);
// -------------------------------------------------------------------------------- 

void test_compare_strings_case_sensitivity(void **state);
// --------------------------------------------------------------------------------

void test_copy_string_nominal(void **state);
// --------------------------------------------------------------------------------

void test_copy_string_empty(void **state);
// --------------------------------------------------------------------------------

void test_copy_string_null(void **state);
// --------------------------------------------------------------------------------

void test_reserve_string_nominal(void **state);
// --------------------------------------------------------------------------------

void test_reserve_string_smaller_size(void **state);
// --------------------------------------------------------------------------------

void test_reserve_string_null(void **state);
// --------------------------------------------------------------------------------

void test_trim_string_nominal(void **state);
// -------------------------------------------------------------------------------- 

void test_trim_already_minimal(void **state);
// -------------------------------------------------------------------------------- 

void test_trim_empty_string(void **state);
// --------------------------------------------------------------------------------

void test_trim_null_string(void **state);
// --------------------------------------------------------------------------------

void test_first_char_occurance_nominal(void **state);
// -------------------------------------------------------------------------------- 

void test_first_char_occurance_not_found(void **state);
// -------------------------------------------------------------------------------- 

void test_first_char_occurance_empty_string(void **state);
// -------------------------------------------------------------------------------- 

void test_first_char_occurance_null(void **state);
// -------------------------------------------------------------------------------- 

void test_last_char_occurance_nominal(void **state);
// -------------------------------------------------------------------------------- 

void test_last_char_occurance_first_char(void **state);
// -------------------------------------------------------------------------------- 

void test_last_char_occurance_not_found(void **state);
// -------------------------------------------------------------------------------- 

void test_last_char_occurance_empty_string(void **state);
// -------------------------------------------------------------------------------- 

void test_last_char_occurance_null(void **state);
// --------------------------------------------------------------------------------

void test_string_start_pointer_nominal(void **state);
// -------------------------------------------------------------------------------- 

void test_string_start_pointer_empty(void **state);
// --------------------------------------------------------------------------------

void test_string_start_pointer_null(void **state);
// --------------------------------------------------------------------------------

void test_string_end_pointer_nominal(void **state);
// -------------------------------------------------------------------------------- 

void test_string_end_pointer_single_char(void **state);
// --------------------------------------------------------------------------------

void test_string_end_pointer_empty(void **state);
// -------------------------------------------------------------------------------- 

void test_string_end_pointer_null(void **state);
// --------------------------------------------------------------------------------

void test_pointer_arithmetic(void **state);
// -------------------------------------------------------------------------------- 

void test_first_substr_literal_nominal(void **state);
// -------------------------------------------------------------------------------- 

void test_first_substr_literal_middle(void **state);
// -------------------------------------------------------------------------------- 

void test_first_substr_literal_not_found(void **state);
// --------------------------------------------------------------------------------

void test_first_substr_literal_empty_substring(void **state);
// --------------------------------------------------------------------------------

void test_first_substr_literal_too_long(void **state);
// -------------------------------------------------------------------------------- 

void test_first_substr_string_nominal(void **state);
// -------------------------------------------------------------------------------- 

void test_first_substr_string_middle(void **state);
// --------------------------------------------------------------------------------

void test_first_substr_string_not_found(void **state);
// --------------------------------------------------------------------------------

void test_first_substr_string_empty(void **state);
// -------------------------------------------------------------------------------- 

void test_first_substr_string_too_long(void **state);
// -------------------------------------------------------------------------------- 

void test_first_substr_null_inputs(void **state);
// --------------------------------------------------------------------------------

void test_last_substr_literal_nominal(void **state);
// --------------------------------------------------------------------------------

void test_last_substr_literal_single_occurrence(void **state);
// --------------------------------------------------------------------------------

void test_last_substr_literal_not_found(void **state);
// -------------------------------------------------------------------------------- 

void test_last_substr_literal_empty_substring(void **state);
// --------------------------------------------------------------------------------

void test_last_substr_literal_too_long(void **state);
// --------------------------------------------------------------------------------

void test_last_substr_string_nominal(void **state);
// --------------------------------------------------------------------------------

void test_last_substr_string_single_occurrence(void **state);
// --------------------------------------------------------------------------------

void test_last_substr_string_not_found(void **state);
// --------------------------------------------------------------------------------

void test_last_substr_string_empty(void **state);
// --------------------------------------------------------------------------------

void test_last_substr_string_too_long(void **state);
// --------------------------------------------------------------------------------

void test_is_string_ptr_within_bounds(void **state);
// --------------------------------------------------------------------------------

void test_is_string_ptr_at_boundaries(void **state);
// --------------------------------------------------------------------------------

void test_is_string_ptr_outside_bounds(void **state);
// --------------------------------------------------------------------------------

void test_is_string_ptr_empty_string(void **state);
// --------------------------------------------------------------------------------

void test_is_string_ptr_null_inputs(void **state);
// --------------------------------------------------------------------------------

void test_drop_substring_literal_nominal(void **state);
// --------------------------------------------------------------------------------

void test_drop_substring_literal_with_spaces(void **state);
// --------------------------------------------------------------------------------

void test_drop_substring_literal_partial_range(void **state);
// --------------------------------------------------------------------------------

void test_drop_substring_string_nominal(void **state);
// --------------------------------------------------------------------------------

void test_drop_substring_string_with_spaces(void **state);
// --------------------------------------------------------------------------------

void test_drop_substring_not_found(void **state);
// --------------------------------------------------------------------------------

void test_drop_substring_invalid_range(void **state);
// -------------------------------------------------------------------------------- 

void test_replace_substr_literal_nominal(void **state);
// --------------------------------------------------------------------------------

void test_replace_substr_literal_shorter_replacement(void **state);
// --------------------------------------------------------------------------------

void test_replace_substr_literal_longer_replacement(void **state);
// -------------------------------------------------------------------------------- 

void test_replace_substr_literal_partial_range(void **state);
// -------------------------------------------------------------------------------- 

void test_replace_substr_string_nominal(void **state);
// -------------------------------------------------------------------------------- 

void test_replace_substr_string_shorter_replacement(void **state);
// -------------------------------------------------------------------------------- 

void test_replace_substr_string_longer_replacement(void **state);
// -------------------------------------------------------------------------------- 

void test_replace_substr_string_partial_range(void **state);
// -------------------------------------------------------------------------------- 

void test_replace_substr_invalid_range(void **state);
// ================================================================================
// ================================================================================ 
#endif /* test_string_H */
// ================================================================================
// ================================================================================
// eof
