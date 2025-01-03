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

#ifndef c_string_H
#define c_string_H

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
// -------------------------------------------------------------------------------- 

/**
 * @brief Tims the string memory to the minimum necessary size 
 *
 * THis function will determine the minimum memory allocation needed to fit 
 * the string and will resize to the minimum memory if oversized.
 *
 * @param str A string container of type string_t
 * @return true if operation is succesful, false otherwise with stderr printout
 */
bool trim_string(string_t* str);
// -------------------------------------------------------------------------------- 

/**
 * @brief Finds the first occurance of a char between two pointers
 *
 * This function deterimines the first appearance of a char value in 
 * a string literal.
 *
 * @param str A pointer to the string_t data type
 * @param value The char value being search for in the string_t data type
 * @return A char pointer to character value or a NULL pointer.
 */
char* first_char_occurance(string_t* str, char value);
// -------------------------------------------------------------------------------- 

/**
 * @brief Finds the last occurance of a char between two pointers
 *
 * This function deterimines the first appearance of a char value in 
 * a string literal.
 *
 * @param str A pointer to the string_t data type
 * @param value The char value being search for in the string_t data type
 * @return A char pointer to character value or a NULL pointer.
 */
char* last_char_occurance(string_t* str, char value);
// -------------------------------------------------------------------------------- 

/**
* @function first_lit_substr_occurance
* @brief Finds the first occurrence of a C string literal substring within a string_t object.
*
* @param str The string_t object to search within
* @param sub_str The C string literal to search for
* @return Pointer to the beginning of the first occurrence of sub_str, or NULL if not found
*         Sets errno to EINVAL if either input is NULL
*/
char* first_lit_substr_occurrence(string_t* str, char* sub_str);
// --------------------------------------------------------------------------------

/**
* @function first_string_substr_occurance
* @brief Finds the first occurrence of a string_t substring within another string_t object.
*
* @param str_1 The string_t object to search within
* @param str_2 The string_t substring to search for
* @return Pointer to the beginning of the first occurrence of str_2, or NULL if not found
*         Sets errno to EINVAL if either input is NULL
*/
char* first_string_substr_occurrence(string_t* str_1, string_t* str_2);
// -------------------------------------------------------------------------------- 

/**
* @macro first_substr_occurance
* @brief A generic macro that selects the appropriate substring search function
*        based on the type of the second argument.
*
* If the second argument is a char*, calls first_lit_substr_occurance.
* If the second argument is a string_t*, calls first_string_substr_occurance.
*
* Example usage:
*     first_substr_occurance(str, "substring")     // Uses literal version
*     first_substr_occurance(str1, str2)          // Uses string_t version
*/
#define first_substr_occurrence(str1, str2) _Generic((str2), \
    char*: first_lit_substr_occurrence, \
    string_t*: first_string_substr_occurrence) (str1, str2)
// -------------------------------------------------------------------------------- 

/**
* @function last_lit_substr_occurance
* @brief Finds the last occurrence of a C string literal substring within a string_t object.
*
* @param str The string_t object to search within
* @param sub_str The C string literal to search for
* @return Pointer to the beginning of the last occurrence of sub_str, or NULL if not found
*         Sets errno to EINVAL if either input is NULL
*/
char* last_lit_substr_occurrence(string_t* str, char* sub_str);
// --------------------------------------------------------------------------------

/**
* @function last_string_substr_occurance
* @brief Finds the last occurrence of a string_t substring within another string_t object.
*
* @param str_1 The string_t object to search within
* @param str_2 The string_t substring to search for
* @return Pointer to the beginning of the last occurrence of str_2, or NULL if not found
*         Sets errno to EINVAL if either input is NULL
*/
char* last_string_substr_occurrence(string_t* str_1, string_t* str_2);
// -------------------------------------------------------------------------------- 

/**
* @macro last_substr_occurance
* @brief A generic macro that selects the appropriate substring search function
*        based on the type of the second argument.
*
* If the second argument is a char*, calls last_lit_substr_occurance.
* If the second argument is a string_t*, calls last_string_substr_occurance.
*
* Example usage:
*     last_substr_occurance(str, "substring")      // Uses literal version
*     last_substr_occurance(str1, str2)           // Uses string_t version
*/
#define last_substr_occurrence(str1, str2) _Generic((str2), \
    char*: last_lit_substr_occurrence, \
    string_t*: last_string_substr_occurrence) (str1, str2)
// --------------------------------------------------------------------------------

/**
* @function first_char
* @brief Returns a pointer to the beginning of a string
*
* @param str A string_t object
* @return Pointer to the beginning of the string NULL if not found
*         Sets errno to EINVAL if either input is NULL
*/
char* first_char(string_t* str);
// --------------------------------------------------------------------------------

/**
* @function last_char
* @brief Returns a pointer to the end of a string
*
* @param str A string_t object
* @return Pointer to the end of the string NULL if not found
*         Sets errno to EINVAL if either input is NULL
*/
char* last_char(string_t* str);
// --------------------------------------------------------------------------------

/**
* @function is_string_ptr
* @brief Deterimes if a pointer is within the bounds of a function
*
* @param str A string_t object
* @param ptr A char pointer 
* @return true if the pointer is within the string bounds, 
*         false otherwise.  If str, str->str, or ptr are NULL,
*         the function will return false and set errno to EINVAL
*/
bool is_string_ptr(string_t* str, char* ptr);
// --------------------------------------------------------------------------------

/**
* @function drop_lit_substr
* @brief Removes all occurrences of a C string literal substring between two pointers.
*
* Searches from end to beginning of the specified range and removes each occurrence
* of the substring, preserving existing spaces between words.
*
* @param string string_t object to modify
* @param substring C string literal to remove
* @param min_ptr Pointer to start of search range within string
* @param max_ptr Pointer to end of search range within string
* @return bool true if successful (including when no matches found), false on error
*         Sets errno to EINVAL if inputs are NULL or range invalid
*         Sets errno to ERANGE if pointers are out of bounds
*/
bool drop_lit_substr(string_t* string, char* substring, char* min_ptr,
                     char* max_ptr);
// -------------------------------------------------------------------------------- 

/**
* @function drop_string_substr
* @brief Removes all occurrences of a string_t substring between two pointers.
*
* Searches from end to beginning of the specified range and removes each occurrence
* of the substring, preserving existing spaces between words.
*
* @param string string_t object to modify
* @param substring string_t object containing substring to remove
* @param min_ptr Pointer to start of search range within string
* @param max_ptr Pointer to end of search range within string
* @return bool true if successful (including when no matches found), false on error
*         Sets errno to EINVAL if inputs are NULL or range invalid
*         Sets errno to ERANGE if pointers are out of bounds
*/
bool drop_string_substr(string_t* string, string_t* substring, char* min_ptr,
                        char* max_ptr);
// -------------------------------------------------------------------------------- 

/**
* @macro drop_substr
* @brief Generic macro that selects appropriate substring removal function based on type.
*
* If the substring is char*, calls drop_lit_substr.
* If the substring is string_t*, calls drop_string_substr.
*
* Example usage:
*     drop_substr(str, "hello", start, end)      // Uses literal version
*     drop_substr(str1, str2, start, end)        // Uses string_t version
*/
#define drop_substr(string, substr, min_ptr, max_ptr) _Generic((substr), \
    char*: drop_lit_substr, \
    string_t*: drop_string_substr) (string, substr, min_ptr, max_ptr)
// ================================================================================ 
// ================================================================================ 
#ifdef __cplusplus
}
// ================================================================================
// ================================================================================ 
#endif /* cplusplus */
#endif /* c_string_H */
// ================================================================================
// ================================================================================
// eof
