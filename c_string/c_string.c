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
// ================================================================================ 
// ================================================================================ 
// PRIVATE FUNCTIONS

static char* _last_literal_between_ptrs(const char* string, char* min_ptr, char* max_ptr) {
    if (!string) {
        fprintf(stderr, "ERROR: Null string provided for last_literal_between_ptrs\n");
        return NULL;
    }
    if (min_ptr >= max_ptr) {
        fprintf(stderr, "ERROR: Min pointer larger than max pointer in last_literal_between_ptrs\n");
        return NULL;
    }
    
    size_t str_len = strlen(string);
    if (str_len == 0) return NULL;
    
    // Ensure we don't search past the end
    char* search_start = max_ptr - str_len + 1;
    if (search_start < min_ptr) return NULL;
    
    for (char* it = search_start; it >= min_ptr; it--) {
        size_t j;
        for (j = 0; j < str_len; j++) {
            if (string[j] != *(it + j)) {
                break;
            }
        }
        if (j == str_len) return it;
    }
    return NULL;
}
// ================================================================================ 
// ================================================================================ 
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
// --------------------------------------------------------------------------------

bool trim_string(string_t* str) {
    if (!str || !str->str) {
        errno = EINVAL;
        return false;
    }
    
    // If already at minimum size, nothing to do
    if (str->len + 1 == str->alloc) {
        return true;
    }
    
    // Sanity check for corrupted string_t
    if (str->len + 1 > str->alloc) {
        errno = EINVAL;
        return false;
    }

    char *ptr = realloc(str->str, str->len + 1);
    if (ptr == NULL) {
        errno = ENOMEM;
        fprintf(stderr, "ERROR: Failure to allocate memory for 'char*' in trim_string()\n");
        return false;
    }
    
    str->str = ptr;
    str->alloc = str->len + 1;
    return true;
}
// -------------------------------------------------------------------------------- 

char* first_char_occurance(string_t* str, char value) {
    if (!str || !str->str) {
        errno = EINVAL;
        return NULL;
    }
    
    char* current = str->str;
    while (*current != '\0') {
        if (*current == value) {
            return current;
        }
        current++;
    }
    
    return NULL;
}
// -------------------------------------------------------------------------------- 

char* last_char_occurance(string_t* str, char value) {
    if (!str || !str->str) {
        errno = EINVAL;
        return NULL;
    }
    
    // Start from last character (before null terminator)
    char* current = str->str + str->len - 1;
    
    // Continue until we reach the beginning of the string
    while (current >= str->str) {
        if (*current == value) {
            return current;
        }
        current--;
    }
    
    return NULL;
}
// --------------------------------------------------------------------------------

char* first_lit_substr_occurrence(string_t* str, char* sub_str) {
    if (!str || !str->str || !sub_str) {
        errno = EINVAL;
        return NULL;
    }
    
    size_t sub_len = strlen(sub_str);
    
    // Check if substring is longer than main string
    if (sub_len > str->len) {
        return NULL;
    }
    
    char* min_ptr = first_char(str);
    // Only need to check up to the point where substring could still fit
    char* max_ptr = min_ptr + (str->len - sub_len);
    
    for (char* it = min_ptr; it <= max_ptr; it++) {
        size_t j;
        for (j = 0; j < sub_len; j++) {
            if (sub_str[j] != *(it + j)) {
                break;
            }
        }
        if (j == sub_len) {
            return it;
        }
    }
    
    return NULL;
}
// -------------------------------------------------------------------------------- 

char* first_string_substr_occurrence(string_t* str, string_t* sub_str) {
    if (!str || !str->str || !sub_str || !sub_str->str) {
        errno = EINVAL;
        return NULL;
    }
    
    size_t sub_len = sub_str->len;
    
    // Check if substring is longer than main string
    if (sub_len > str->len) {
        return NULL;
    }
    
    char* min_ptr = first_char(str);
    // Only need to check up to the point where substring could still fit
    char* max_ptr = min_ptr + (str->len - sub_len);
    
    for (char* it = min_ptr; it <= max_ptr; it++) {
        size_t j;
        for (j = 0; j < sub_len; j++) {
            if (sub_str->str[j] != *(it + j)) {
                break;
            }
        }
        if (j == sub_len) {
            return it;
        }
    }
    
    return NULL;
}
// --------------------------------------------------------------------------------

char* last_lit_substr_occurrence(string_t* str, char* sub_str) {
    if (!str || !str->str || !sub_str) {
        errno = EINVAL;
        return NULL;
    }
    
    size_t sub_len = strlen(sub_str);
    
    // Check if substring is longer than main string
    if (sub_len > str->len) {
        return NULL;
    }
    
    char* min_ptr = first_char(str);
    // Only need to check up to the point where substring could still fit
    char* max_ptr = min_ptr + (str->len - sub_len);
    
    for (char* it = max_ptr; it >= min_ptr; it--) {
        size_t j;
        for (j = 0; j < sub_len; j++) {
            if (sub_str[j] != *(it + j)) {
                break;
            }
        }
        if (j == sub_len) {
            return it;
        }
    }
    
    return NULL;
}
// -------------------------------------------------------------------------------- 

char* last_string_substr_occurrence(string_t* str, string_t* sub_str) {
   if (!str || !str->str || !sub_str || !sub_str->str) {
       errno = EINVAL;
       return NULL;
   }
   
   size_t sub_len = sub_str->len;
   
   // Check if substring is longer than main string
   if (sub_len > str->len) {
       return NULL;
   }
   
   char* min_ptr = first_char(str);
   // Only need to check up to the point where substring could still fit
   char* max_ptr = min_ptr + (str->len - sub_len);
   
   for (char* it = max_ptr; it >= min_ptr; it--) {
       size_t j;
       for (j = 0; j < sub_len; j++) {
           if (sub_str->str[j] != *(it + j)) {
               break;
           }
       }
       if (j == sub_len) {
           return it;
       }
   }
   
   return NULL;
}
// --------------------------------------------------------------------------------

char* first_char(string_t* str) {
    if (!str || !str->str) {
        errno = EINVAL;
        return NULL;
    }
    char* begin = str->str;
    return begin;
}
// --------------------------------------------------------------------------------

char* last_char(string_t* str) {
    if (!str || !str->str) {
        errno = EINVAL;
        return NULL;
    }
    char* begin = str->str + str->len - 1;
    return begin;
}
// --------------------------------------------------------------------------------

bool is_string_ptr(string_t* str, char* ptr) {
    if (!str || !str->str || !ptr) {
        errno = EINVAL;
        return false;  // Changed from NULL to false
    }
    
    char* start = first_char(str);
    char* end = start + str->len;  // Points one past the last character
    
    return (ptr >= start && ptr < end);
}
// -------------------------------------------------------------------------------- 

bool drop_lit_substr(string_t* string, const char* substring, char* min_ptr, char* max_ptr) {
    if (!string || !substring || !string->str) {
        errno = EINVAL;
        return false;
    }
    if (!is_string_ptr(string, min_ptr) || !is_string_ptr(string, max_ptr)) {
        errno = ERANGE;
        return false;
    }
    if (max_ptr <= min_ptr) {
        errno = EINVAL;
        return false;
    }
    
    size_t substr_len = strlen(substring);
    if (string->len < substr_len) return true;
    
    char* ptr;
    while ((ptr = _last_literal_between_ptrs(substring, min_ptr, max_ptr))) {
        size_t drop_len = substr_len;
        // Check if there's a space after the substring and it's within bounds
        if (ptr + substr_len < string->str + string->len && 
            *(ptr + substr_len) == ' ') {
            drop_len++;  // Include the space
        }
        
        size_t move_length = max_ptr - (ptr + drop_len) + 1;
        memmove(ptr, ptr + drop_len, move_length);
        
        string->len -= drop_len;
        max_ptr -= drop_len;
        *(string->str + string->len) = '\0';
    }
    return true;
}
// --------------------------------------------------------------------------------

bool drop_string_substr(string_t* string, const string_t* substring, char* min_ptr, char* max_ptr) {
    if (!string || !substring || !string->str || !substring->str) {
        errno = EINVAL;
        return false;
    }
    if (!is_string_ptr(string, min_ptr) || !is_string_ptr(string, max_ptr)) {
        errno = ERANGE;
        return false;
    }
    if (max_ptr <= min_ptr) {
        errno = EINVAL;
        return false;
    }
    
    size_t substr_len = substring->len;
    if (string->len < substr_len) return true;
    
    char* ptr;
    while ((ptr = _last_literal_between_ptrs(substring->str, min_ptr, max_ptr))) {
        size_t drop_len = substr_len;
        // Check if there's a space after the substring and it's within bounds
        if (ptr + substr_len < string->str + string->len && 
            *(ptr + substr_len) == ' ') {
            drop_len++;  // Include the space
        }
        
        size_t move_length = max_ptr - (ptr + drop_len) + 1;
        memmove(ptr, ptr + drop_len, move_length);
        
        string->len -= drop_len;
        max_ptr -= drop_len;
        *(string->str + string->len) = '\0';
    }
    return true;
}
// -------------------------------------------------------------------------------- 

bool replace_lit_substr(string_t* string, const char* pattern, const char* replace_string,
                        char* min_ptr, char* max_ptr) {
    if (!string || !string->str || !pattern ||  !replace_string || 
        !min_ptr || !max_ptr) {
        errno = EINVAL;
        return false;
    }
   
    if (!is_string_ptr(string, min_ptr) || !is_string_ptr(string, max_ptr)) {
        errno = ERANGE;
        return false;
    } 

    if (min_ptr > max_ptr) {
        errno = ERANGE;
        return false;
    }
    
    // Calculate the delta between the pattern and replacement lengths.
    int delta = strlen(replace_string) - strlen(pattern);

    size_t pattern_len = strlen(pattern);
    size_t replace_len = strlen(replace_string);

    // Pre-allocate if we need more space
    size_t count = 0;
    if (delta > 0) {
        char* search_ptr = min_ptr;
        char* end_ptr = max_ptr;
        while ((search_ptr = _last_literal_between_ptrs(pattern, min_ptr, end_ptr))) {
            count++;
            if (search_ptr <= min_ptr) break;  // Found match at start, we're done
            end_ptr = search_ptr - 1;
        } 
        size_t new_size = string->len + (delta * count) + 1;
        if (new_size > string->alloc) {
            size_t min_pos = min_ptr - string->str;
            size_t max_pos = max_ptr - string->str;
            
            char* new_data = realloc(string->str, new_size);
            if (!new_data) {
                errno = ENOMEM;
                fprintf(stderr, "ERROR: Realloc failed in replace_lit_substring\n");
                return false;
            }
            string->str = new_data;
            string->alloc = new_size;
            min_ptr = string->str + min_pos;
            max_ptr = string->str + max_pos;
        }
    }
    
    char* ptr = _last_literal_between_ptrs(pattern, min_ptr, max_ptr);
    while (ptr) {
        // If the replacement string is the same length, copy it over.
        if (delta == 0) {
            memcpy(ptr, replace_string, replace_len);
            max_ptr = ptr;
        }

        // - If replacement string is smaller, copy string, move memory to the 
        //   left, and update length
        else if (delta < 0) {
            memcpy(ptr, replace_string, replace_len);

            size_t tail_length = string->str + string->len - (ptr + pattern_len);
            memmove(ptr + replace_len, ptr + pattern_len, tail_length);

            string->len += delta; // delta is negative, so it reduces string->len

            string->str[string->len] = '\0';

            max_ptr -= pattern_len - replace_len;
        }
        else {
            memmove(ptr + replace_len,
                    ptr + pattern_len,
                    string->len - (ptr - string->str) - pattern_len);
            memcpy(ptr, replace_string, replace_len);
            string->len += delta;
            string->str[string->len] = '\0';
            ptr += delta;
            max_ptr = ptr;
        }        
         // Find the next pattern instance within the updated pointer bounds.
         if (min_ptr < max_ptr)
            ptr = _last_literal_between_ptrs(pattern, min_ptr, max_ptr);
         else
            ptr = NULL;
    }

    return true;
}
// --------------------------------------------------------------------------------

bool replace_string_substr(string_t* string, const string_t* pattern, const string_t* replace_string,
                           char* min_ptr, char* max_ptr) {
    if (!string || !string->str || !pattern || !pattern->str || 
        !replace_string || !replace_string->str || !min_ptr || !max_ptr) {
        errno = EINVAL;
        return false;
    }
  
    if (!is_string_ptr(string, min_ptr) || !is_string_ptr(string, max_ptr)) {
        errno = ERANGE;
        return false;
    }

    if (min_ptr > max_ptr) {
        errno = ERANGE;
        return false;
    }
   
    // Calculate the delta between the pattern and replacement lengths.
    int delta = replace_string->len - pattern->len;
   
    // Count occurrences within specified range if we need to grow the string
    size_t count = 0;
    if (delta > 0) {
        char* search_ptr = min_ptr;
        char* end_ptr = max_ptr;
        while ((search_ptr = _last_literal_between_ptrs(pattern->str, min_ptr, end_ptr))) {
            count++;
            if (search_ptr <= min_ptr) break;  // Found match at start, we're done
            end_ptr = search_ptr - 1;
        }
       
        // Pre-allocate all needed memory
        size_t new_size = string->len + (delta * count) + 1;
        if (new_size > string->alloc) {
            size_t min_pos = min_ptr - string->str;
            size_t max_pos = max_ptr - string->str;
           
            char* new_data = realloc(string->str, new_size);
            if (!new_data) {
                errno = ENOMEM;
                fprintf(stderr, "ERROR: Realloc failed in replace_string_substring\n");
                return false;
            }
            string->str = new_data;
            string->alloc = new_size;
            min_ptr = string->str + min_pos;
            max_ptr = string->str + max_pos;
        }
    }
   
    char* ptr = _last_literal_between_ptrs(pattern->str, min_ptr, max_ptr);
    while (ptr) {
        if (delta == 0) {
            memcpy(ptr, replace_string->str, replace_string->len);
        }
        else if (delta < 0) {
            memcpy(ptr, replace_string->str, replace_string->len);
            memmove(ptr + replace_string->len, 
                    ptr + pattern->len,
                    string->len - (ptr - string->str) - pattern->len + 1);
            string->len += delta;
        }
        else {  // delta > 0
            memmove(ptr + replace_string->len,
                    ptr + pattern->len,
                    string->len - (ptr - string->str) - pattern->len + 1);
            memcpy(ptr, replace_string->str, replace_string->len);
            string->len += delta;
        }
       
        max_ptr = ptr - 1;
        if (min_ptr >= max_ptr) break;
        ptr = _last_literal_between_ptrs(pattern->str, min_ptr, max_ptr);
    }
   
    string->str[string->len] = '\0';
    return true;
}
// --------------------------------------------------------------------------------

void to_upper_char(char* val) {
    if (!val) {
        errno = EINVAL;
        return;
    }
    if (*val >= 'a' && *val <= 'z') *val -= 32; 
}
// --------------------------------------------------------------------------------

void to_lower_char(char* val) {
    if (!val) {
        errno = EINVAL;
        return;
    }
    if (*val >= 'A' && *val <= 'Z') *val += 32; 
}
// --------------------------------------------------------------------------------

void to_uppercase(string_t *s) {
    if(!s || !s->str) {
        errno = EINVAL;
        return;
    }
    char* begin = s->str;
    char* end = s->str + s->len;
    for (char* i =  begin; i != end; i++) {
        if (*i >= 'a' && *i <= 'z') *i -= 32;
    }
}
// --------------------------------------------------------------------------------

void to_lowercase(string_t *s) {
    if(!s || !s->str) {
        errno = EINVAL;
        return;
    }
    char* begin = s->str;
    char* end = s->str + s->len;
    for (char* i =  begin; i != end; i++) {
        if (*i >= 'A' && *i <= 'Z') *i += 32;
    }
}
// ================================================================================
// ================================================================================
// eof
