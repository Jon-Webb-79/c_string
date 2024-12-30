// ================================================================================
// ================================================================================
// - File:    cstring.c
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

#include "include/c_string.h"

#include <errno.h>  // For errno and strerror 
#include <stdlib.h> // For size_t, malloc, and realloc
#include <string.h> // For strerror
#include <limits.h> // For INT_MIN
// ================================================================================ 
// ================================================================================ 
// STRING_T DATA TYPE 

struct string_t {
    char* str;
    size_t len;
    size_t alloc;
};
// --------------------------------------------------------------------------------

string_t* init_string(const char* str) {
    if (str == NULL) {
        errno = EINVAL;
        return NULL;
    }

    // Scan for null terminator
    const char* end = str;
    while (*end != '\0') end++;
    size_t len = end - str;

    string_t* ptr = malloc(sizeof(string_t));
    if (!ptr) {
        errno = ENOMEM;
        fprintf(stderr, "ERROR: Failure to allocate memory for 'string_t' in init_string()\n"); 
        return NULL;
    }

    char* ptr2 = malloc(len + 1);
    if (!ptr2) {
        errno = ENOMEM;
        fprintf(stderr, "ERROR: Failure to allocate memory for 'char*' in init_string()\n");
        free(ptr);
        return NULL;
    }

    memcpy(ptr2, str, len);
    ptr2[len] = '\0';
    
    ptr->str = ptr2;
    ptr->len = len;
    ptr->alloc = len + 1;
    return ptr;
}
// --------------------------------------------------------------------------------

void free_string(string_t* str) {
    if (!str) {
        errno = EINVAL;
        return;
    }
    if (str->str) {
        free(str->str);
    }
    free(str);
}
// --------------------------------------------------------------------------------

void _free_string(string_t** str) {
    if (str && *str) {
        free_string(*str);
        *str = NULL;
    }
}
// --------------------------------------------------------------------------------

const char* get_string(const string_t* str) {
    if (!str || !str->str) {
        errno = EINVAL;
        return NULL;
    }
    return str->str;
}
// --------------------------------------------------------------------------------

const size_t string_size(const string_t* str) {
    if (!str || !str->str) {
        errno = EINVAL;
        return LONG_MAX;
    }
    return str->len;
}
// --------------------------------------------------------------------------------

const size_t string_alloc(const string_t* str) {
    if (!str || !str->str) {
        errno = EINVAL;
        return LONG_MAX;
    }
    return str->alloc;
}
// --------------------------------------------------------------------------------

bool string_string_concat(string_t* str1, const string_t* str2) {
    if (!str1 || !str2 || !str1->str || !str2->str) {
        errno = EINVAL;
        return false;
    }

    // Calculate the new required length
    size_t new_len = str1->len + str2->len;

    // Check if the current buffer can hold the concatenated string
    if (new_len + 1 > str1->alloc) { // +1 for the null terminator
        // Reallocate the buffer to accommodate the new string
        char* new_buffer = realloc(str1->str, new_len + 1); // +1 for the null terminator
        if (!new_buffer) {
            errno = ENOMEM;
            fprintf(stderr, "ERROR: Failed to reallocate memory for char* in string_string_concat()\n");
            return false;
        }
        str1->str = new_buffer;
        str1->alloc = new_len + 1;
    }

    // Append the second string to the first
    strncat(str1->str, str2->str, str2->len);

    // Update the length of the first string
    str1->len = new_len;
    return true;
}
// --------------------------------------------------------------------------------

bool string_lit_concat(string_t* str1, const char* literal) {
    if (!str1 || !str1->str || !literal) {
        errno = EINVAL;
        return false;
    }

    // Calculate the new required length
    size_t literal_len = strlen(literal);
    size_t new_len = str1->len + literal_len;

    // Check if the current buffer can hold the concatenated string
    if (new_len + 1 > str1->alloc) { // +1 for the null terminator
        // Reallocate the buffer to accommodate the new string
        char* new_buffer = realloc(str1->str, new_len + 1); // +1 for the null terminator
        if (!new_buffer) {
            errno = ENOMEM;
            fprintf(stderr, "ERROR: Failed to reallocate memory for char* in string_lit_concat()\n");
            return false;
        }
        str1->str = new_buffer;
        str1->alloc = new_len + 1;
    }

    // Append the string literal to the first string
    strncat(str1->str, literal, literal_len);

    // Update the length of the first string
    str1->len = new_len;

    return true; // Indicate success
}
// --------------------------------------------------------------------------------

int compare_strings_lit(const string_t* str_struct, const char* string) {
    if (!str_struct || !string || !str_struct->str) {
        errno = EINVAL;
        return INT_MIN; // Or another designated error value
    }

    size_t string_len = strlen(string);
    size_t min_len = (str_struct->len < string_len) ? str_struct->len : string_len;

    for (size_t i = 0; i < min_len; i++) {
        if (str_struct->str[i] != string[i]) {
            return (unsigned char)str_struct->str[i] - (unsigned char)string[i];
        }
    }
    return str_struct->len - string_len;
}
// --------------------------------------------------------------------------------

int compare_strings_string(const string_t* str_struct_one, string_t* str_struct_two) {
    if (!str_struct_one || !str_struct_two || !str_struct_one->str || !str_struct_two->str) {
        errno = EINVAL;
        return INT_MIN; // Or another designated error value
    } 

    size_t string_len = str_struct_two->len;
    size_t min_len = (str_struct_one->len < string_len) ? str_struct_one->len : string_len;

    for (size_t i = 0; i < min_len; i++) {
        if (str_struct_one->str[i] != str_struct_two->str[i]) {
            return (unsigned char)str_struct_one->str[i] - (unsigned char)str_struct_two->str[i];
        }
    }
    return str_struct_one->len - string_len;
}
// --------------------------------------------------------------------------------

string_t* copy_string(const string_t* str) {
    if (!str || !str->str) {
        errno = EINVAL;
        return NULL;
    }
    string_t* new_str = init_string(get_string(str));
    if (new_str->alloc < str->alloc) 
        reserve_string(new_str, str->alloc);
    return new_str; 
}
// --------------------------------------------------------------------------------

bool reserve_string(string_t* str, size_t len) {
    if (!str || !str->str) {
        errno = EINVAL;
        return false;
    }

    // Ensure the requested length is greater than the current allocation
    if (len <= str->alloc) {
        errno = EINVAL;
        return false;
    }

    // Attempt to reallocate memory
    char* ptr = realloc(str->str, sizeof(char) * len);
    if (!ptr) {
        errno = ENOMEM;
        fprintf(stderr,"ERROR: Failed to reallocate memory for char* in reserver_string()\n");
        return false;
    }

    // Update the string_t structure with the new allocation
    str->str = ptr;
    str->alloc = len;

    return true;
}
// ================================================================================
// ================================================================================
// eof
