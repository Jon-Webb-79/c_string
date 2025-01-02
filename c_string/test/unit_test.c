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
// ================================================================================ 
// ================================================================================
// Begin code

int main(int argc, const char * argv[]) {
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
	};
	return cmocka_run_group_tests(test_string, NULL, NULL);
}
// ================================================================================
// ================================================================================
// eof

