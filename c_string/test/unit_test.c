// ================================================================================
// ================================================================================
// - File:    unit_test.c
// - Purpose: Describe the file purpose here
//
// Source Metadata
// - Author:  Jonathan A. Webb
// - Date:    December 30, 2024
// - Version: 1.0
// - Copyright: Copyright 2024, Jon Webb Inc.
// ================================================================================
// ================================================================================
// Include modules here

#include <stdio.h>
#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <stdint.h>
#include <cmocka.h>

#include "test_string.h"
#include "test_vector.h"
// ================================================================================ 
// ================================================================================
// Begin code

const struct CMUnitTest test_string[] = {
    cmocka_unit_test(test_init_string_nominal),
    cmocka_unit_test(test_init_string_empty),
    cmocka_unit_test(test_init_string_null),
    cmocka_unit_test(test_free_string_nominal),
    cmocka_unit_test(test_free_string_null),
    cmocka_unit_test(test_init_string_special_chars),
    cmocka_unit_test(test_init_string_long),
    cmocka_unit_test(test_getters_null),
    cmocka_unit_test(test_string_cleanup_gbc),
    cmocka_unit_test(test_concat_string_nominal),
    cmocka_unit_test(test_concat_literal_nominal),
    cmocka_unit_test(test_concat_empty_string),
    cmocka_unit_test(test_concat_empty_literal),
    cmocka_unit_test(test_concat_null_inputs),
    cmocka_unit_test(test_concat_reallocation),
    cmocka_unit_test(test_concat_multiple_times),
    cmocka_unit_test(test_concat_special_characters),
    cmocka_unit_test(test_concat_large_strings),
    cmocka_unit_test(test_compare_strings_equal),
    cmocka_unit_test(test_compare_strings_less),
    cmocka_unit_test(test_compare_strings_greater),
    cmocka_unit_test(test_compare_strings_different_lengths),
    cmocka_unit_test(test_compare_strings_empty),
    cmocka_unit_test(test_compare_strings_null),
    cmocka_unit_test(test_compare_strings_case_sensitivity), 
    cmocka_unit_test(test_copy_string_nominal),
    cmocka_unit_test(test_copy_string_empty),
    cmocka_unit_test(test_copy_string_null),
    cmocka_unit_test(test_reserve_string_nominal),
    cmocka_unit_test(test_reserve_string_smaller_size),
    cmocka_unit_test(test_reserve_string_null),
    cmocka_unit_test(test_trim_string_nominal),
    cmocka_unit_test(test_trim_already_minimal),
    cmocka_unit_test(test_trim_empty_string),
    cmocka_unit_test(test_first_char_occurance_nominal),
    cmocka_unit_test(test_first_char_occurance_not_found),
    cmocka_unit_test(test_first_char_occurance_empty_string),
    cmocka_unit_test(test_first_char_occurance_null),
    cmocka_unit_test(test_last_char_occurance_nominal),
    cmocka_unit_test(test_last_char_occurance_first_char),
    cmocka_unit_test(test_last_char_occurance_not_found),
    cmocka_unit_test(test_last_char_occurance_empty_string),
    cmocka_unit_test(test_last_char_occurance_null),
    cmocka_unit_test(test_string_start_pointer_nominal),
    cmocka_unit_test(test_string_start_pointer_empty),
    cmocka_unit_test(test_string_start_pointer_null),
    cmocka_unit_test(test_string_end_pointer_nominal),
    cmocka_unit_test(test_string_end_pointer_single_char),
    cmocka_unit_test(test_string_end_pointer_empty),
    cmocka_unit_test(test_string_end_pointer_null),
    cmocka_unit_test(test_pointer_arithmetic),
    cmocka_unit_test(test_first_substr_literal_nominal),
    cmocka_unit_test(test_first_substr_literal_middle),
    cmocka_unit_test(test_first_substr_literal_not_found),
    cmocka_unit_test(test_first_substr_literal_empty_substring),
    cmocka_unit_test(test_first_substr_literal_too_long),
    cmocka_unit_test(test_first_substr_string_nominal),
    cmocka_unit_test(test_first_substr_string_middle),
    cmocka_unit_test(test_first_substr_string_not_found),
    cmocka_unit_test(test_first_substr_string_empty),
    cmocka_unit_test(test_first_substr_string_too_long),
    cmocka_unit_test(test_last_substr_literal_nominal),
    cmocka_unit_test(test_last_substr_literal_single_occurrence),
    cmocka_unit_test(test_last_substr_literal_not_found),
    cmocka_unit_test(test_last_substr_literal_empty_substring),
    cmocka_unit_test(test_last_substr_literal_too_long),
    cmocka_unit_test(test_last_substr_string_single_occurrence),
    cmocka_unit_test(test_last_substr_string_not_found),
    cmocka_unit_test(test_last_substr_string_empty),
    cmocka_unit_test(test_last_substr_string_too_long),
    cmocka_unit_test(test_is_string_ptr_within_bounds),
    cmocka_unit_test(test_is_string_ptr_at_boundaries),
    cmocka_unit_test(test_is_string_ptr_outside_bounds),
    cmocka_unit_test(test_is_string_ptr_empty_string),
    cmocka_unit_test(test_is_string_ptr_null_inputs),
    cmocka_unit_test(test_drop_substring_literal_nominal),
    cmocka_unit_test(test_drop_substring_literal_with_spaces),
    cmocka_unit_test(test_drop_substring_literal_partial_range),
    cmocka_unit_test(test_drop_substring_string_nominal),
    cmocka_unit_test(test_drop_substring_string_with_spaces),
    cmocka_unit_test(test_drop_substring_not_found),
    cmocka_unit_test(test_drop_substring_invalid_range),
    cmocka_unit_test(test_replace_substr_literal_nominal),
    cmocka_unit_test(test_replace_substr_literal_shorter_replacement),
    cmocka_unit_test(test_replace_substr_literal_longer_replacement),
    cmocka_unit_test(test_replace_substr_literal_partial_range),
    cmocka_unit_test(test_replace_substr_string_nominal),
    cmocka_unit_test(test_replace_substr_string_shorter_replacement),
    cmocka_unit_test(test_replace_substr_string_longer_replacement),
    cmocka_unit_test(test_replace_substr_string_partial_range),
    cmocka_unit_test(test_replace_substr_invalid_range),
    cmocka_unit_test(test_lowercase_char_nominal),
    cmocka_unit_test(test_uppercase_char_nominal),
    cmocka_unit_test(test_lowercase_char_null_value),
    cmocka_unit_test(test_uppercase_char_nominal),
    cmocka_unit_test(test_uppercase_string_nominal),
    cmocka_unit_test(test_lowercase_string_nominal),
    cmocka_unit_test(test_uppercase_string_null),
    cmocka_unit_test(test_lowercase_string_null),
    cmocka_unit_test(test_pop_token_nominal),
    cmocka_unit_test(test_pop_token_multiple_tokens),
    cmocka_unit_test(test_pop_token_at_end),
    cmocka_unit_test(test_pop_token_no_token),
    cmocka_unit_test(test_pop_token_empty_string),
    cmocka_unit_test(test_pop_token_null_input),
    cmocka_unit_test(test_pop_token_single_char),
    cmocka_unit_test(test_pop_token_consecutive_tokens),
    cmocka_unit_test(test_token_count_single_delimiter),
    cmocka_unit_test(test_token_count_multiple_delimiters),
    cmocka_unit_test(test_token_count_consecutive_delimiters),
    cmocka_unit_test(test_token_count_leading_trailing_delimiters),
    cmocka_unit_test(test_token_count_single_token),
    cmocka_unit_test(test_token_count_empty_string),
    cmocka_unit_test(test_token_count_null_inputs),
    cmocka_unit_test(test_token_count_only_delimiters),
    cmocka_unit_test(test_token_count_mixed_delimiters),
    cmocka_unit_test(test_get_char_nominal),
    cmocka_unit_test(test_get_char_out_of_bounds),
    cmocka_unit_test(test_get_char_null),
    cmocka_unit_test(test_replace_char_nominal),
    cmocka_unit_test(test_replace_char_out_of_bounds),
    cmocka_unit_test(test_replace_char_null),
    cmocka_unit_test(test_replace_char_special_chars),
    cmocka_unit_test(test_trim_leading_nominal),
    cmocka_unit_test(test_trim_leading_multiple_types),
    cmocka_unit_test(test_trim_leading_no_whitespace),
    cmocka_unit_test(test_trim_trailing_nominal),
    cmocka_unit_test(test_trim_trailing_multiple_types),
    cmocka_unit_test(test_trim_trailing_no_whitespace),
    cmocka_unit_test(test_trim_all_nominal),
    cmocka_unit_test(test_trim_all_multiple_types),
    cmocka_unit_test(test_trim_all_no_whitespace),
    cmocka_unit_test(test_string_iterator_forward),
    cmocka_unit_test(test_string_iterator_reverse),
    cmocka_unit_test(test_string_iterator_empty_string),
    cmocka_unit_test(test_string_iterator_null_string),
    cmocka_unit_test(test_string_iterator_bounds),
};
// --------------------------------------------------------------------------------

const struct CMUnitTest test_string_vector[] = {
	cmocka_unit_test(test_str_vector_init),
    cmocka_unit_test(test_str_vector_push_back),
    cmocka_unit_test(test_str_vector_auto_resize),
    cmocka_unit_test(test_str_vector_null_inputs),
    cmocka_unit_test(test_str_vector_empty_string),
    cmocka_unit_test(test_str_vector_multiple_pushes),
    cmocka_unit_test(test_str_vector_gbc),
    cmocka_unit_test(test_push_front_empty_vector),
    cmocka_unit_test(test_push_front_existing_elements),
    cmocka_unit_test(test_push_front_reallocation),
    cmocka_unit_test(test_push_front_multiple),
    cmocka_unit_test(test_push_front_null_inputs),
    cmocka_unit_test(test_push_front_empty_string),
    cmocka_unit_test(test_insert_str_vector_middle),
    cmocka_unit_test(test_insert_str_vector_start),
    cmocka_unit_test(test_insert_str_vector_end),
    cmocka_unit_test(test_insert_str_vector_reallocation),
    cmocka_unit_test(test_insert_str_vector_invalid_index),
    cmocka_unit_test(test_insert_str_vector_null_inputs),
    cmocka_unit_test(test_insert_str_vector_empty_string),
};
// ================================================================================ 
// ================================================================================ 
int main(int argc, const char * argv[]) {
    int status;
    status = cmocka_run_group_tests(test_string, NULL, NULL);
    if (status != 0) 
        return status;	
    status = cmocka_run_group_tests(test_string_vector, NULL, NULL);
	return status;
}
// ================================================================================
// ================================================================================
// eof

