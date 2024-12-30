// ================================================================================
// ================================================================================
// - File:    cstring.h
// - Purpose: Describe the file purpose here
//
// Source Metadata
// - Author:  Jonathan A. Webb
// - Date:    December 30, 2024
// - Version: 1.0
// - Copyright: Copyright 2024, Jon Webb Inc.
// ================================================================================
// ================================================================================

#ifndef cstring_H
#define cstring_H

#include <stdio.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif
// ================================================================================ 
// ================================================================================ 

/**
 * @struct string_t
 * @brief Forward declaration for a dynamic data structure for storing strings.
 *
 * Fields:
 *  - chart* str: Pointer to a string literal
 *  - size_t len: The current number of elements in the arrays.
 *  - size_t alloc: The total allocated capacity of the arrays.
 */
typedef struct string_t string_t;
// --------------------------------------------------------------------------------

/**
 * @function init_string
 * @brief Allocates and initializes a dynamically allocated string_t object.
 *
 * The function initializes the string_t structure with the contents of the provided
 * string, copying the string into dynamically allocated memory.
 *
 * @param str A null-terminated C string to initialize the string_t object with.
 * @return A pointer to the initialized string_t object, or NULL on failure.
 *         Sets errno to ENOMEM if memory allocation fails or EINVAL if `str` is NULL.
 */
string_t* init_string(const char* str);
// --------------------------------------------------------------------------------

/**
 * @function free_string
 * @brief Frees all memory associated with a string_t object.
 *
 * The function releases the memory for the internal string and the string_t structure itself.
 *
 * @param str A pointer to the string_t object to be freed.
 * @return void. Logs an error if `str` is NULL.
 */
void free_string(string_t* str);
// -------------------------------------------------------------------------------- 

/**
 * @function _free_string
 * @brief A helper function for use with cleanup attributes to free string_t objects.
 *
 * This function frees the memory of a string_t object and sets the pointer to NULL.
 *
 * @param str A double pointer to the string_t object to be freed.
 * @return void.
 */
void _free_string(string_t** str);
// --------------------------------------------------------------------------------

#if defined(__GNUC__) || defined (__clang__)
    /**
     * @macro STRING_GBC
     * @brief A macro for enabling automatic cleanup of string_t objects.
     *
     * This macro uses the cleanup attribute to automatically call `_free_string`
     * when the scope ends, ensuring proper memory management.
     */
    #define STRING_GBC __attribute__((cleanup(_free_string)))
#endif
// --------------------------------------------------------------------------------
/**
 * @function get_string
 * @brief Retrieves the C string stored in a string_t object.
 *
 * @param str A pointer to the string_t object.
 * @return A pointer to the null-terminated C string stored in the object,
 *         or NULL if `str` is NULL or invalid. Sets errno to EINVAL on error.
 */
const char* get_string(const string_t* str);
// --------------------------------------------------------------------------------

/**
 * @function string_size
 * @brief Retrieves the current length of the string stored in a string_t object.
 *
 * @param str A pointer to the string_t object.
 * @return The length of the string in bytes (excluding the null terminator),
 *         or -1 on error. Sets errno to EINVAL if `str` is NULL.
 */
const size_t string_size(const string_t* str);
// --------------------------------------------------------------------------------

/**
 * @function string_alloc
 * @brief Retrieves the total allocated capacity of the string in a string_t object.
 *
 * @param str A pointer to the string_t object.
 * @return The total allocated capacity in bytes, or -1 on error.
 *         Sets errno to EINVAL if `str` is NULL.
 */
const size_t string_alloc(const string_t* str);
// --------------------------------------------------------------------------------

/**
 * @function string_string_concat
 * @brief Concatenates the contents of one string_t object to another.
 *
 * @param str1 A pointer to the destination string_t object.
 * @param str2 A pointer to the source string_t object.
 * @return true if successful, false on failure. Sets errno to ENOMEM if memory
 *         allocation fails or EINVAL if either input is NULL.
 */
bool string_string_concat(string_t* str, const string_t* string);
// -------------------------------------------------------------------------------- 

/**
 * @function string_lit_concat
 * @brief Concatenates a string literal to a string_t object.
 *
 * @param str1 A pointer to the destination string_t object.
 * @param literal A null-terminated C string to append to the string_t object.
 * @return true if successful, false on failure. Sets errno to ENOMEM if memory
 *         allocation fails or EINVAL if either input is NULL.
 */
bool string_lit_concat(string_t* str, const char* string);
// --------------------------------------------------------------------------------

/**
 * @macro string_concat
 * @brief A generic macro that selects the appropriate concatenation function
 *        based on the type of the second argument.
 *
 * If the second argument is a `char*`, it calls `string_lit_concat`.
 * Otherwise, it calls `string_string_concat`.
 */
#define string_concat(str_one, str_two) _Generic((str_two), \
    char*: string_lit_concat, \
    default: string_string_concat) (str_one, str_two)
// --------------------------------------------------------------------------------

/**
 * @function compare_strings_lit
 * @brief Compares a string_t object with a string literal.
 *
 * The comparison is lexicographical and case-sensitive.
 *
 * @param str_struct A pointer to the string_t object.
 * @param string A null-terminated C string to compare with.
 * @return An integer less than, equal to, or greater than zero if the string_t
 *         is found, respectively, to be less than, to match, or to be greater than `string`.
 *         Returns INT_MIN on error (sets errno to EINVAL).
 */
int compare_strings_lit(const string_t* str_struct, const char* string);
// --------------------------------------------------------------------------------

/**
 * @function compare_strings_string
 * @brief Compares two string_t objects.
 *
 * The comparison is lexicographical and case-sensitive.
 *
 * @param str_struct_one A pointer to the first string_t object.
 * @param str_struct_two A pointer to the second string_t object.
 * @return An integer less than, equal to, or greater than zero if the first string_t
 *         is found, respectively, to be less than, to match, or to be greater than the second string_t.
 *         Returns INT_MIN on error (sets errno to EINVAL).
 */
int compare_strings_string(const string_t* str_struct_one, string_t* str_struct_two);
// --------------------------------------------------------------------------------

/**
 * @macro compare_strings
 * @brief A generic macro that selects the appropriate string comparison function
 *        based on the type of the second argument.
 *
 * If the second argument is a `char*`, it calls `compare_strings_lit`.
 * Otherwise, it calls `compare_strings_string`.
 */
#define compare_strings(str_one, str_two) _Generic((str_two), \
    char*: compare_strings_lit, \
    default: compare_strings_string) (str_one, str_two)
// --------------------------------------------------------------------------------

/**
 * @function copy_string
 * @brief Creates a deep copy of a string data type
 *
 * @param str A string_t data type
 * @return A string_t data type with the exact contents of str
 */
string_t* copy_string(const string_t *str);
// --------------------------------------------------------------------------------

/**
 * @function reserve_string
 * @brief Reserves memory for the string_t data type to avoid constant memory allocations
 *
 * Will not allow the user to reserve less memory than exists at function call 
 *
 * @param str A string_t data type
 * @param len The buffer length to allocate in string
 * @return true of allocation is successful, false otherwise
 */
bool reserve_string(string_t* str, size_t len);
// ================================================================================ 
// ================================================================================ 
#ifdef __cplusplus
}
// ================================================================================
// ================================================================================ 
#endif /* cplusplus */
#endif /* cstring_H */
// ================================================================================
// ================================================================================
// eof
