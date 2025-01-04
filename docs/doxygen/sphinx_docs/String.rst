String Functions
================

The c_string library provides a dynamic string type and associated functions 
for safe string manipulation in C.  All of the functions described in this 
section can be found in the ``c_string.h`` header file.

Data Types
----------

string_t
~~~~~~~~
A dynamic string type that manages memory allocation automatically and provides safe string operations.
The internal structure is opaque to users, preventing direct manipulation of the memory.

While the data structure is opaque to the user, the struct has this 
format

.. code-block:: c

   typedef struct {
       char* str;
       size_t len;
       size_t alloc;
    }


Core Functions
--------------
The functions and Macros in this section are core to the creation and 
desctruction of ``size_t`` data types.

Initialization and Memory Management
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
The functions and Macros in this section are used to control the creation,
memory allocation, and specific destruction of ``size_t`` data types.

init_string
^^^^^^^^^^^
.. c:function:: string_t* init_string(const char* str)

   Creates and initializes a new ``string_t`` object from a C-style string. If
   compiled with ``gcc`` of ``clang`` compilers, consider using :ref:`GBC_STRING
   macro <string_cleanup_macro>`.  If the developer wishes to Pre-allocate 
   a larger amount of memory than would be normally allocated with this function, 
   they can also call the :ref:`reserve_string <reserve-string-func>` function 
   after initialization.

   :param str: A null-terminated C string
   :returns: Pointer to new ``string_t`` object, or NULL on failure
   :raises: Sets errno to ENOMEM if memory allocation fails or EINVAL if str is NULL

   Example:

   .. code-block:: c

      string_t* str = init_string("Hello, World!");
      /*
       * if compiling with gcc or clang consider using 
       * string_t* str STRING_GBC init_string("Hello, World!");
       */
      if (str == NULL) {
          fprintf(stderr, "Failed to initialize string\n");
          return 1;
      }
      printf("String content: %s\n", get_string(str));
      // If STRING_GBC macro is used, their is no need to call free_string()
      free_string(str);

   Output::

      String content: Hello, World!

.. _reserve-string-func:

reserve_string
^^^^^^^^^^^^^^
.. c:function:: bool reserve_string(string_t* str, size_t len)

  Pre-allocates memory for a ``string_t`` object to optimize for future concatenations.
  Will not reduce allocation size below current size.

  :param str: ``string_t`` object to reserve memory for
  :param len: Desired buffer length in bytes
  :returns: true if successful, false if len is less than current allocation or on error
  :raises: Sets errno to EINVAL if str is NULL or len is too small, ENOMEM if allocation fails

  Example:

  .. code-block:: c

     string_t* str STRING_GBC = init_string("Hello");
     if (str) {
         printf("Initial allocation: %zu\n", string_alloc(str));
         
         // Reserve more space
         if (reserve_string(str, 20)) {
             printf("After reserve(20): %zu\n", string_alloc(str));
             
             // Try to reserve less space (should fail)
             if (!reserve_string(str, 10)) {
                 printf("Failed to reduce allocation as expected\n");
             }
             
             // Content remains unchanged
             printf("String content: %s\n", get_string(str));
         }
     }

  Output::

     Initial allocation: 6
     After reserve(20): 20
     Failed to reduce allocation as expected
     String content: Hello

trim_string
^^^^^^^^^^^
.. c:function:: bool trim_string(string_t* str)

  Reduces the allocated memory of a ``string_t`` object to the minimum required size
  (string length plus null terminator). This is useful for optimizing memory usage
  after string operations that might have left excess allocated space.

  :param str: ``string_t`` object to trim
  :returns: true if successful or already at minimum size, false on error
  :raises: Sets errno to EINVAL if str is NULL or corrupted, ENOMEM if reallocation fails

  Example:

  .. code-block:: c

     string_t* str STRING_GBC = init_string("Hello");
     if (str) {
         // First reserve extra space
         reserve_string(str, 20);
         printf("Before trim - Content: %s, Size: %zu, Allocated: %zu\n",
                get_string(str), string_size(str), string_alloc(str));
         
         // Now trim the excess space
         if (trim_string(str)) {
             printf("After trim  - Content: %s, Size: %zu, Allocated: %zu\n",
                    get_string(str), string_size(str), string_alloc(str));
         }
     }

  Output::

     Before trim - Content: Hello, Size: 5, Allocated: 20
     After trim  - Content: Hello, Size: 5, Allocated: 6

free_string
^^^^^^^^^^^
.. c:function:: void free_string(string_t* str)

   Deallocates all memory associated with a string_t object.

   :param str: Pointer to string_t object to free
   :raises: Sets errno to EINVAL if str is NULL

   Example:

   .. code-block:: c

      string_t* str = init_string("Hello");
      // Use the string...
      free_string(str);
      str = NULL;  // Good practice to avoid dangling pointers

Automatic Cleanup
~~~~~~~~~~~~~~~~~
In general the C language does not allow automated garbage collection of 
memory that is out of scope.  This section describes a poor mans 
garbage collection within the C language, for the ``string_t`` data type,
that can only be enabled if compiled with ``gcc`` or ``clang``.

.. _string_cleanup_macro:

STRING_GBC
^^^^^^^^^^
.. c:macro:: STRING_GBC

   Enables automatic cleanup of ``string_t`` objects when they go out of scope.
   Available only with ``GCC`` and ``Clang`` compilers.  If this option is available 
   for your compiler, this is the preferred method for memory management.

   Example:

   .. code-block:: c

      void example_function(void) {
          STRING_GBC string_t* str = init_string("Hello");
          if (!str) {
              return;
          }
          printf("String: %s\n", get_string(str));
          // No need to call free_string - cleanup happens automatically
      }

   Output::

      String: Hello

String Access Functions
-----------------------
The functions discussed in this section are used to access the string within 
the ``string_t`` data type as well as the allocated memory and size of a string.

get_string
~~~~~~~~~~
.. c:function:: const char* get_string(const string_t* str)

  Retrieves the C string stored in a ``string_t`` object.

  :param str: Pointer to the string_t object
  :returns: Pointer to the null-terminated string, or NULL on failure
  :raises: Sets errno to EINVAL if str is NULL

  Example:

  .. code-block:: c

     string_t* str STRING_GBC = init_string("Hello, World!");
     // If not compiled with gcc or clang, string_t* str = init_string("Hello, World!");
     if (str) {
         const char* content = get_string(str);
         if (content) {
             printf("String content: %s\n", content);
         }
         // If not compiled with gcc or clang, free_string(str);
     }

  Output::

     String content: Hello, World!

string_size
~~~~~~~~~~~
.. c:function:: const size_t string_size(const string_t* str)

  Returns the length of the string (number of characters excluding null terminator).
  This is the equivalant of the ``strlen`` function from the ``string.h`` header 
  file; however, this function is safely bounded by the length of the string 
  and is not prone to buffer overflow attacks.

  :param str: Pointer to the ``string_t`` object
  :returns: Length of string, or LONG_MAX on failure
  :raises: Sets errno to EINVAL if str is NULL

  Example:

  .. code-block:: c

     string_t* str STRING_GBC = init_string("Hello");
     // If not compiled with gcc or clang, string_t* str = init_string("Hello");
     if (str) {
         size_t len = string_size(str);
         if (len != LONG_MAX) {
             printf("String: %s\nLength: %zu\n", get_string(str), len);
         }
         // If not compiled with gcc or clang, free_string(str);
     }

  Output::

     String: Hello
     Length: 5

string_alloc
~~~~~~~~~~~~
.. c:function:: const size_t string_alloc(const string_t* str)

  Returns the total allocated capacity of the string buffer. 

  :param str: Pointer to the ``string_t`` object
  :returns: Allocated capacity in bytes, or LONG_MAX on failure
  :raises: Sets errno to EINVAL if str is NULL

  Example:

  .. code-block:: c

     string_t* str STRING_GBC = init_string("Test");
     // If not compiled with gcc or clang, string_t* str = init_string("Test");
     if (str) {
         printf("String: %s\n", get_string(str));
         printf("Length: %zu\n", string_size(str));
         printf("Allocated: %zu\n", string_alloc(str));
         // If not compiled with gcc or clang, free_string(str);
     }

  Output::

     String: Test
     Length: 4
     Allocated: 5

String Manipulation Functions
-----------------------------
The functions and Macros within this section allow a user to manipulate the 
string data within the ``string_t`` data type.

Concatenation Functions and Macros 
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
These functions and macros allow a user to concatenate data to a ``string_t``
data type with a string literal or another ``string_t`` data type.  These 
functions are essentially the equivalent of a ``push_back`` function for the 
``string_t`` data type.

string_string_concat
^^^^^^^^^^^^^^^^^^^^
.. c:function:: bool string_string_concat(string_t* str1, const string_t* str2)

  Concatenates two ``string_t`` objects, appending the second string to the first.
  Developers should consider using the :ref:`string_concat macro <string-concat-macro>`
  in place of the ``string_string_concat`` function. This function is bounded by
  the length of the second string and is not prone to buffer overflow attacks.

  :param str1: Destination string_t object
  :param str2: Source string_t object to append
  :returns: true if successful, false on failure
  :raises: Sets errno to ENOMEM on allocation failure or EINVAL if either input is NULL

  Example:

  .. code-block:: c

     string_t* str1 STRING_GBC = init_string("Hello ");
     string_t* str2 STRING_GBC = init_string("World!");
     if (str1 && str2) {
         printf("Before: %s\n", get_string(str1));
         if (string_string_concat(str1, str2)) {
             printf("After:  %s\n", get_string(str1));
         }
     }

  Output::

     Before: Hello 
     After:  Hello World!

string_lit_concat
^^^^^^^^^^^^^^^^^
.. c:function:: bool string_lit_concat(string_t* str1, const char* literal)

  Concatenates a C string literal to a ``string_t`` object.Developers should consider 
  using the :ref:`string_concat macro <string-concat-macro>` in place of the 
  ``string_lit_concat`` function. 

  :param str1: Destination string_t object
  :param literal: C string to append
  :returns: true if successful, false on failure
  :raises: Sets errno to ENOMEM on allocation failure or EINVAL if either input is NULL

  Example:

  .. code-block:: c

     string_t* str STRING_GBC = init_string("Hello ");
     if (str) {
         printf("Before: %s\n", get_string(str));
         if (string_lit_concat(str, "World!")) {
             printf("After:  %s\n", get_string(str));
         }
     }

  Output::

     Before: Hello 
     After:  Hello World!

.. _string-concat-macro:

string_concat Macro
^^^^^^^^^^^^^^^^^^^
.. c:macro:: string_concat(str_one, str_two)

  A generic macro that automatically selects the appropriate concatenation function
  based on the type of the second argument.

  Example:

  .. code-block:: c

     string_t* str1 STRING_GBC = init_string("Hello ");
     string_t* str2 STRING_GBC = init_string("World ");
     
     if (str1 && str2) {
         printf("Initial: %s\n", get_string(str1));
         
         // Using macro with string_t
         string_concat(str1, str2);
         printf("After string_t concat: %s\n", get_string(str1));
         
         // Using macro with literal
         string_concat(str1, "!!!");
         printf("After literal concat: %s\n", get_string(str1));
     }

  Output::

     Initial: Hello 
     After string_t concat: Hello World 
     After literal concat: Hello World !!!

Drop Substring Functions and Macros 
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
The functions and Macros in this section are used to search the char data 
within a ``string_t`` data type for a sub-string.  If the sub-string is found 
it is removed from the string and the data is shifted an appropriate amount 
to form a contiguous char array within the ``string_t`` data type.

.. _drop-lit-substr-func:

drop_lit_substr
^^^^^^^^^^^^^^^
.. c:function:: bool drop_lit_substr(string_t* string, const char* substring, char* min_ptr, char* max_ptr)

  Removes all occurrences of a C string literal substring between two pointers in a ``string_t`` object.
  Searches from end to beginning and preserves existing spaces between words. 
  The :ref:`drop_substr <drop-substr-macro>` generic Macro can also be used in place of this function. 

  :param string: ``string_t`` object to modify
  :param substring: C string literal to remove
  :param min_ptr: Pointer to start of search range
  :param max_ptr: Pointer to end of search range
  :returns: true if successful (including no matches found), false on error
  :raises: Sets errno to EINVAL if inputs are NULL or range invalid, ERANGE if pointers out of bounds

  Example:

  .. code-block:: c

     string_t* str STRING_GBC = init_string("hello world hello there hello");
     printf("Initial length: %ld\n", string_size(str));
     char* start = first_char(str);
     char* end = last_char(str);
     
     if (drop_lit_substr(str, "hello", start, end)) {
         printf("Result: '%s'\n", get_string(str));
         printf("Final length: %ld\n", string_size(str));
     }

  Output::

     Initial Length: 29
     Result: 'world there '
     Final length: 11

.. _drop-string-substr-func:

drop_string_substr
^^^^^^^^^^^^^^^^^^
.. c:function:: bool drop_string_substr(string_t* string, const string_t* substring, char* min_ptr, char* max_ptr)

  Removes all occurrences of a ``string_t`` substring between two pointers in another string_t object.
  Searches from end to beginning and preserves existing spaces between words.
  The :ref:`drop_substr <drop-substr-macro>` generic Macro can also be used in 
  place of this function.

  :param string: string_t object to modify
  :param substring: string_t object containing substring to remove
  :param min_ptr: Pointer to start of search range
  :param max_ptr: Pointer to end of search range
  :returns: true if successful (including no matches found), false on error
  :raises: Sets errno to EINVAL if inputs are NULL or range invalid, ERANGE if pointers out of bounds

  Example:

  .. code-block:: c

     string_t* str STRING_GBC = init_string("hello world hello there hello");
     string_t* sub STRING_GBC = init_string("hello");
     printf("Initial length: %ld\n", string_size(str));
     char* start = first_char(str);
     char* end = last_char(str);
     
     if (drop_string_substr(str, sub, start, end)) {
         printf("Result: '%s'\n", get_string(str));
         printf("Final length: %ld\n", string_size(str));
     }

  Output::

     Initial Length: 29
     Result: 'world there '
     Final length: 11

.. _drop-substr-macro:

drop_substr
^^^^^^^^^^^
.. c:macro:: drop_substr(string, substr, min_ptr, max_ptr)

  A generic macro that selects the appropriate substring removal function based on
  the type of the second argument. Provides a unified interface for removing substrings
  regardless of the substring type.  This macro is a wrapper around the 
  :ref:`drop_lit_substr() <drop-lit-substr-func>` and 
  :ref:`drop_string_substr() <drop-string-substr-func>` functions.

  Example:

  .. code-block:: c

     string_t* str = init_string("test hello test hello test");
     string_t* sub STRING_GBC = init_string("hello");
     char* start = first_char(str);
     char* end = last_char(str);
     
     // Using with literal
     printf("Using literal - Before: '%s'\n", get_string(str));
     drop_substr(str, "hello", start, end);
     printf("After: '%s'\n", get_string(str));
     
     // Reset string
     free_string(str);
     str STRING_GBC = init_string("test hello test hello test");
     
     // Using with string_t
     printf("\nUsing string_t - Before: '%s'\n", get_string(str));
     drop_substr(str, sub, start, end);
     printf("After: '%s'\n", get_string(str));

  Output::

     Using literal - Before: 'test hello test hello test'
     After: 'test test test'
     
     Using string_t - Before: 'test hello test hello test'
     After: 'test test test'

String Replacement Functions
~~~~~~~~~~~~~~~~~~~~~~~~~~~~
The functions described in this section can be used to replace sub string 
ocurrences over a user defined range with a new sub-string.

.. _replace-lit-substring-func:

replace_lit_substr
^^^^^^^^^^^^^^^^^^
.. c:function:: bool replace_lit_substr(string_t* string, const char* pattern, const char* replace_string, char* min_ptr, char* max_ptr)

  Replaces all occurrences of a C string literal pattern with a replacement string between
  two specified pointers in a ``string_t`` object. Searches from right to left.
  Developers may also choose to implement the :ref:`replace_substring <replace-substring-macro>`
  Macro in place of this function.

  :param string: ``string_t`` object to modify
  :param pattern: C string literal to search for and replace
  :param replace_string: C string literal to replace pattern with
  :param min_ptr: Pointer to start of search range
  :param max_ptr: Pointer to end of search range
  :returns: true if successful (including no matches found), false on error
  :raises: Sets errno to EINVAL if inputs are NULL, ERANGE if pointers out of bounds

  Example:

  .. code-block:: c

     string_t* str STRING_GBC = init_string("hello world hello there hello");
     char* start = first_char(str);
     char* end = last_char(str);
     
     // Replace all "hello" with "hi" in the entire string
     if (replace_lit_substr(str, "hello", "hi", start, end)) {
         printf("After replacement: '%s'\n", get_string(str));
     }
     
  Output::

     After replacement: 'hi world hi there hi'

.. _replace-string-substring-func:

replace_string_substr
^^^^^^^^^^^^^^^^^^^^^
.. c:function:: bool replace_string_substr(string_t* string, const string_t* pattern, const string_t* replace_string, char* min_ptr, char* max_ptr)

  Replaces all occurrences of a ``string_t`` pattern with another ``string_t`` between
  two specified pointers in a ``string_t object``. Searches from right to left.
  Developers may also choose to implement the :ref:`replace_substring <replace-substring-macro>`
  Macro in place of this function.

  :param string: ``string_t`` object to modify
  :param pattern: ``string_t`` object containing pattern to search for
  :param replace_string: ``string_t`` object containing replacement string
  :param min_ptr: Pointer to start of search range
  :param max_ptr: Pointer to end of search range
  :returns: true if successful (including no matches found), false on error
  :raises: Sets errno to EINVAL if inputs are NULL, ERANGE if pointers out of bounds

  Example:

  .. code-block:: c

     string_t* str STRING_GBC = init_string("hello world hello there");
     string_t* pattern STRING_GBC = init_string("hello");
     string_t* replacement STRING_GBC = init_string("hi");
     
     char* start = first_char(str) + 6;  // Start after first "hello"
     char* end = last_char(str);
     
     // Replace "hello" with "hi" only in specified range
     if (replace_string_substr(str, pattern, replacement, start, end)) {
         printf("After partial replacement: '%s'\n", get_string(str));
     }
     
  Output::

     After partial replacement: 'hello world hi there'

.. _replace-substring-macro:

replace_substr
^^^^^^^^^^^^^^
.. c:macro:: replace_substr(string, pattern, replace_string, min_ptr, max_ptr)

  A generic macro that selects the appropriate replacement function based on the type
  of the pattern argument. Provides a unified interface for string replacement operations.
  This macro wraps the :ref:`replace_string_substr() <replace-string-substring-func>`
  and :ref:`replace_lit_substr() <replace-lit-substring-func>` functions

  Example:

  .. code-block:: c

     string_t* str = init_string("hello world hello there");
     string_t* pat = init_string("hello");
     string_t* rep = init_string("hi");
     char* start = first_char(str);
     char* end = last_char(str);
     
     // Using with literal strings
     replace_substr(str, "hello", "hi", start, end);
     printf("Using literals: '%s'\n", get_string(str));
     
     // Reset string
     free_string(str);
     str = init_string("hello world hello there");
     
     // Using with string_t objects
     replace_substr(str, pat, rep, start, end);
     printf("Using string_t: '%s'\n", get_string(str));
     
     free_string(str);
     free_string(pat);
     free_string(rep);

  Output::

     Using literals: 'hi world hi there'
     Using string_t: 'hi world hi there'

String Comparison Functions
---------------------------
The functions and Macros in this section are used to compare to strings 
for equivalance or for their differences.

compare_strings_lit
~~~~~~~~~~~~~~~~~~~
.. c:function:: int compare_strings_lit(const string_t* str_struct, const char* string)

  Compares a ``string_t`` object with a C string literal lexicographically.
  Developers should consider using the :ref:`compare_strings macro <compare-strings-macro>`
  in place of the ``compare_strings_lit`` function. This is the equivalent of the 
  ``strcmp`` function from the ``string.h`` header file.  However, this function 
  is bounded by the size of the first string and is not subject to buffer overflow 
  attacks.

  :param str_struct: ``string_t`` object to compare
  :param string: C string literal to compare against
  :returns: < 0 if str_struct is less than string, 
           0 if equal, 
           > 0 if str_struct is greater than string,
           INT_MIN on error
  :raises: Sets errno to EINVAL if either input is NULL

  Example:

  .. code-block:: c

     string_t* str = init_string("hello");
     if (str) {
         printf("Comparing '%s' with 'hello': %d\n", 
                get_string(str), compare_strings_lit(str, "hello"));
         printf("Comparing '%s' with 'world': %d\n", 
                get_string(str), compare_strings_lit(str, "world"));
         printf("Comparing '%s' with 'apple': %d\n", 
                get_string(str), compare_strings_lit(str, "apple"));
         free_string(str);
     }

  Output::

     Comparing 'hello' with 'hello': 0
     Comparing 'hello' with 'world': -15
     Comparing 'hello' with 'apple': 7

compare_strings_string
~~~~~~~~~~~~~~~~~~~~~~
.. c:function:: int compare_strings_string(const string_t* str_struct_one, string_t* str_struct_two)

  Compares two ``string_t`` objects lexicographically. Developers should consider 
  using the :ref:`compare-strings macro <compare-strings-macro>` in place of the 
  ``compare_strings_string`` function. This is hte equivalent of the ``strcmp``
  function from the ``string.h`` header file; however, it is bounded by the length 
  of the strings and is not subject to buffer overflow attacks.

  :param str_struct_one: First ``string_t`` object to compare
  :param str_struct_two: Second ``string_t`` object to compare against
  :returns: < 0 if str_struct_one is less than str_struct_two, 
           0 if equal, 
           > 0 if str_struct_one is greater than str_struct_two,
           INT_MIN on error
  :raises: Sets errno to EINVAL if either input is NULL

  Example:

  .. code-block:: c

     string_t* str1 = init_string("hello");
     string_t* str2 = init_string("world");
     string_t* str3 = init_string("hello");
     
     if (str1 && str2 && str3) {
         printf("Comparing '%s' with '%s': %d\n", 
                get_string(str1), get_string(str2), 
                compare_strings_string(str1, str2));
         printf("Comparing '%s' with '%s': %d\n", 
                get_string(str1), get_string(str3), 
                compare_strings_string(str1, str3));
         
         free_string(str1);
         free_string(str2);
         free_string(str3);
     }

  Output::

     Comparing 'hello' with 'world': -15
     Comparing 'hello' with 'hello': 0

.. _compare-strings-macro:

compare_strings Macro
~~~~~~~~~~~~~~~~~~~~~
.. c:macro:: compare_strings(str_one, str_two)

  A generic macro that automatically selects the appropriate comparison function
  based on the type of the second argument.  This macro is the equivalent of the 
  ``strcmp`` function from the ``string.h`` header file and is bounded by the size 
  of the first string, and is not subject to buffer overflow attacks.

  Example:

  .. code-block:: c

     string_t* str1 = init_string("hello");
     string_t* str2 = init_string("world");
     
     if (str1 && str2) {
         // Compare with string_t
         printf("Compare with string_t: %d\n", 
                compare_strings(str1, str2));
         
         // Compare with literal
         printf("Compare with literal: %d\n", 
                compare_strings(str1, "hello"));
         
         free_string(str1);
         free_string(str2);
     }

  Output::

     Compare with string_t: -15
     Compare with literal: 0

String Utility Functions
------------------------
The functions and Macros in this section offer general utility functions 
for characterizing data within the ``size_t`` data type.

copy_string
~~~~~~~~~~~
.. c:function:: string_t* copy_string(const string_t* str)

  Creates a deep copy of a ``string_t`` object, preserving both content and allocation size.

  :param str: ``string_t`` object to copy
  :returns: New string_t object with identical content, or NULL on failure
  :raises: Sets errno to EINVAL if str is NULL or ENOMEM if allocation fails

  Example:

  .. code-block:: c

     string_t* original = init_string("Hello World");
     if (original) {
         printf("Original: %s\n", get_string(original));
         
         string_t* copy = copy_string(original);
         if (copy) {
             printf("Copy: %s\n", get_string(copy));
             printf("Original size: %zu\n", string_size(original));
             printf("Copy size: %zu\n", string_size(copy));
             printf("Original allocation: %zu\n", string_alloc(original));
             printf("Copy allocation: %zu\n", string_alloc(copy));
             
             free_string(copy);
         }
         free_string(original);
     }

  Output::

     Original: Hello World
     Copy: Hello World
     Original size: 11
     Copy size: 11
     Original allocation: 12
     Copy allocation: 12

first_char_occurance
~~~~~~~~~~~~~~~~~~~~
.. c:function:: char* first_char_occurance(string_t* str, char value)

  Finds the first occurrence of a character in a ``string_t`` object.
  Similar to ``strchr()`` from the C standard library.

  :param str: string_t object to search in
  :param value: Character to search for
  :returns: Pointer to the first occurrence of the character, or NULL if not found
  :raises: Sets errno to EINVAL if str is NULL

  Example:

  .. code-block:: c

     string_t* str = init_string("Hello World");
     if (str) {
         char* result = first_char_occurance(str, 'l');
         if (result) {
             printf("First 'l' found at position: %ld\n", result - get_string(str));
             printf("Remaining string from 'l': %s\n", result);
         }
         
         // Try finding a character that doesn't exist
         if (!first_char_occurance(str, 'z')) {
             printf("Character 'z' not found\n");
         }
         
         free_string(str);
     }

  Output::

     First 'l' found at position: 2
     Remaining string from 'l': llo World
     Character 'z' not found

last_char_occurance
~~~~~~~~~~~~~~~~~~~
.. c:function:: char* last_char_occurance(string_t* str, char value)

  Finds the last occurrence of a character in a ``string_t`` object.
  Similar to ``strrchr()`` from the C standard library.

  :param str: string_t object to search in
  :param value: Character to search for
  :returns: Pointer to the last occurrence of the character, or NULL if not found
  :raises: Sets errno to EINVAL if str is NULL

Example:

.. code-block:: c

   string_t* str = init_string("Hello World");
   if (str) {
       char* first = first_char_occurance(str, 'l');
       char* last = last_char_occurance(str, 'l');

       // ptrdiff_t can be accessed from the stddef.h header file
       if (first && last) {
           ptrdiff_t first_pos = first - get_string(str);
           ptrdiff_t last_pos = last - get_string(str);
           printf("First 'l' at position: %td\n", first_pos);
           printf("Last 'l' at position: %td\n", last_pos);
           printf("Number of characters between: %td\n", last - first);
       }
       
       free_string(str);
   }

Output::

   First 'l' at position: 2
   Last 'l' at position: 9
   Number of characters between: 7

first_char
~~~~~~~~~~
.. c:function:: char* first_char(string_t* str)

  Returns a pointer to the beginning of the string stored in a ``string_t`` object.
  Similar to begin() in C++ string class.

  :param str: ``string_t`` object to get start pointer from
  :returns: Pointer to the first character of the string, or NULL on error
  :raises: Sets errno to EINVAL if str is NULL

  Example:

  .. code-block:: c

     string_t* str STRING_GBC = init_string("Hello World");
     if (str) {
         char* start = first_char(str);
         if (start) {
             printf("First character: %c\n", *start);
             printf("First two chars: %.2s\n", start);
         }
     }

  Output::

     First character: H
     First two chars: He

last_char
~~~~~~~~~
.. c:function:: char* last_char(string_t* str)

  Returns a pointer to the last character of the string stored in a ``string_t`` object.
  Note: This points to the last actual character, not the null terminator.

  :param str: ``string_t`` object to get end pointer from
  :returns: Pointer to the last character of the string, or NULL on error
  :raises: Sets errno to EINVAL if str is NULL

  Example:

  .. code-block:: c

     string_t* str STRING_GBC = init_string("Hello World");
     if (str) {
         char* start = first_char(str);
         char* end = last_char(str);
         
         if (start && end) {
             printf("First character: %c\n", *start);
             printf("Last character: %c\n", *end);
             printf("String length: %td\n", end - start + 1);
         }
     }

  Output::

     First character: H
     Last character: d
     String length: 11

.. _first-lit-substr-func:

First Occurance of a Substring 
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
The functions and Macros in this section offer a utility to search for the 
first occurrence of a sub-string within a ``string_t`` data type.  These functions 
and Macros return the location of the sub-string start point as a ``char`` pointer.

first_lit_substr_occurance
^^^^^^^^^^^^^^^^^^^^^^^^^^
.. c:function:: char* first_lit_substr_occurance(string_t* str, char* sub_str)

  Finds the first occurrence of a C string literal substring within a ``string_t`` object.
  Similar to strstr() from the C standard library.  The user can also implement 
  :ref:`first_substr_occurance <first-substr-macro>` macro in place of this function.

  :param str: ``string_t`` object to search within
  :param sub_str: C string literal to search for
  :returns: Pointer to first character of substring if found, NULL otherwise
  :raises: Sets errno to EINVAL if either input is NULL

  Example:

  .. code-block:: c

     string_t* str = init_string("hello world hello there");
     char* result = first_lit_substr_occurance(str, "world");
     
     if (result) {
         printf("Found 'world' at position: %td\n", result - get_string(str));
         printf("Remaining string: %s\n", result);
     }
     
     free_string(str);

  Output::

     Found 'world' at position: 6
     Remaining string: world hello there

.. _first-str-substr-func:

first_string_substr_occurrence
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
.. c:function:: char* first_string_substr_occurrence(string_t* str, string_t* sub_str)

  Finds the first occurrence of a ``string_t`` substring within another ``string_t`` object.
  The developer can also implement the :ref:`first_substr_occurance() <first-substr-macro>`
  Macro in place of this function.

  :param str: ``string_t`` object to search within
  :param sub_str: ``string_t`` object containing the substring to search for
  :returns: Pointer to first character of substring if found, NULL otherwise
  :raises: Sets errno to EINVAL if either input is NULL

  Example:

  .. code-block:: c

     string_t* str = init_string("hello world hello there");
     string_t* sub = init_string("world");
     
     char* result = first_string_substr_occurrence(str, sub);
     if (result) {
         printf("Found substring at position: %td\n", result - get_string(str));
     }
     
     free_string(str);
     free_string(sub);

  Output::

     Found substring at position: 6

.. _first-substr-macro:

first_substr_occurrence
^^^^^^^^^^^^^^^^^^^^^^^
.. c:macro:: first_substr_occurance(str1, str2)

  A generic macro that selects the appropriate substring search function based on
  the type of the second argument. Provides a unified interface for substring
  searching regardless of the substring type.  This macro wraps the 
  :ref:`first_lit_substr_occurrence() <first-lit-substr-func>` and 
  :ref:`first_string_substr_occurrence() <first-str-substr-func>` functions

  Example:

  .. code-block:: c

     string_t* str = init_string("hello world hello there");
     string_t* sub = init_string("world");
     
     // Using with string literal
     char* result1 = first_substr_occurrence(str, "hello");
     if (result1) {
         printf("Found 'hello' at position: %td\n", result1 - get_string(str));
     }
     
     // Using with string_t
     char* result2 = first_substr_occurance(str, sub);
     if (result2) {
         printf("Found 'world' at position: %td\n", result2 - get_string(str));
     }
     
     free_string(str);
     free_string(sub);

  Output::

     Found 'hello' at position: 0
     Found 'world' at position: 6

Last Occurance of a Substring 
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
The functions and Macros in this section offer a utility to search for the 
last occurrence of a sub-string within a ``string_t`` data type.  These functions 
and Macros return the location of the sub-string start point as a ``char`` pointer.

.. _last-lit-substr-func:

last_lit_substr_occurrence
^^^^^^^^^^^^^^^^^^^^^^^^^^
.. c:function:: char* last_lit_substr_occurrence(string_t* str, char* sub_str)

  Finds the last occurrence of a C string literal substring within a ``string_t`` object.
  Similar to strstr() but searches from end to beginning.  Developer can use the 
  generic :ref:`last_substr_occurrence() <last-substr-macro>` macro in place of this function.

  :param str: ``string_t`` object to search within
  :param sub_str: C string literal to search for
  :returns: Pointer to first character of last substring match if found, NULL otherwise
  :raises: Sets errno to EINVAL if either input is NULL

  Example:

  .. code-block:: c

     string_t* str = init_string("hello world hello there");
     char* result = last_lit_substr_occurrence(str, "hello");
     
     if (result) {
         printf("Last 'hello' found at position: %td\n", result - get_string(str));
         printf("Remaining string: %s\n", result);
     }
     
     free_string(str);

  Output::

     Last 'hello' found at position: 12
     Remaining string: hello there

.. _last-string-substr-func:

last_string_substr_occurrence
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
.. c:function:: char* last_string_substr_occurrence(string_t* str, string_t* sub_str)

  Finds the last occurrence of a ``string_t`` substring within another ``string_t`` object.
  Developer can use the generic :ref:`last_substr_occurrence() <last-substr-macro>` 
  macro in place of this function.

  :param str: ``string_t`` object to search within
  :param sub_str: ``string_t`` object containing the substring to search for
  :returns: Pointer to first character of last substring match if found, NULL otherwise
  :raises: Sets errno to EINVAL if either input is NULL

  Example:

  .. code-block:: c

     string_t* str = init_string("hello world hello there");
     string_t* sub = init_string("hello");
     
     char* result = last_string_substr_occurrence(str, sub);
     if (result) {
         printf("Last occurrence found at position: %td\n", result - get_string(str));
         printf("Text from match onwards: %s\n", result);
     }
     
     free_string(str);
     free_string(sub);

  Output::

     Last occurrence found at position: 12
     Text from match onwards: hello there

.. _last-substr-macro:

last_substr_occurrence
^^^^^^^^^^^^^^^^^^^^^^
.. c:macro:: last_substr_occurrence(str1, str2)

  A generic macro that selects the appropriate substring search function based on
  the type of the second argument. Provides a unified interface for finding the
  last occurrence of a substring regardless of the substring type.  This macro 
  wraps the :ref:`last_lit_substr_occurrence() <last-lit-substr-func>` 
  and :ref:`last_string_substr_occurrence() <last-string-substr-func>` functions.

  Example:

  .. code-block:: c

     string_t* str = init_string("hello world hello there hello");
     string_t* sub = init_string("hello");
     
     // Using with string literal
     char* result1 = last_substr_occurrence(str, "hello");
     if (result1) {
         printf("Last 'hello' using literal at: %td\n", result1 - get_string(str));
     }
     
     // Using with string_t
     char* result2 = last_substr_occurrence(str, sub);
     if (result2) {
         printf("Last 'hello' using string_t at: %td\n", result2 - get_string(str));
     }
     
     free_string(str);
     free_string(sub);

  Output::

     Last 'hello' using literal at: 18
     Last 'hello' using string_t at: 18

is_string_ptr
~~~~~~~~~~~~~
.. c:function:: bool is_string_ptr(string_t* str, char* ptr)

  Determines if a pointer falls within the valid bounds of a ``string_t`` object's string data.
  This is useful for validating pointers returned by string search functions or pointer
  arithmetic operations.

  :param str: ``string_t`` object containing the string data
  :param ptr: Pointer to check
  :returns: true if pointer is within string bounds, false otherwise
  :raises: Sets errno to EINVAL if either input is NULL

  Example:

  .. code-block:: c

     string_t* str STRING_GBC = init_string("hello world");
     // Get a pointer to the middle of the string
     char* middle = first_char(str) + 5;  // Points to space
     
     if (is_string_ptr(str, middle)) {
         printf("Pointer is within string bounds\n");
         printf("Character at pointer: '%c'\n", *middle);
     }
     
     // Check a pointer outside bounds
     char* beyond = first_char(str) + string_size(str);  // Points to null terminator
     if (!is_string_ptr(str, beyond)) {
         printf("Pointer is outside string bounds\n");
     }
     

  Output::

     Pointer is within string bounds
     Character at pointer: ' '
     Pointer is outside string bounds

  Note:
     The valid range includes all characters from the first character up to, but not
     including, the null terminator. A pointer equal to str->str + str->len (pointing
     to the null terminator) is considered out of bounds.
