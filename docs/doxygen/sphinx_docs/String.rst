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

Initialization and Memory Management
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

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

get_string
^^^^^^^^^^
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
^^^^^^^^^^^
.. c:function:: const size_t string_size(const string_t* str)

  Returns the length of the string (number of characters excluding null terminator).

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
^^^^^^^^^^^^
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

String Concatenation Functions
------------------------------

string_string_concat
^^^^^^^^^^^^^^^^^^^^
.. c:function:: bool string_string_concat(string_t* str1, const string_t* str2)

  Concatenates two ``string_t`` objects, appending the second string to the first.
  Developers should consider using the :ref:`string_concat macro <string-concat-macro>`
  in place of the ``string_string_concat`` function.

  :param str1: Destination string_t object
  :param str2: Source string_t object to append
  :returns: true if successful, false on failure
  :raises: Sets errno to ENOMEM on allocation failure or EINVAL if either input is NULL

  Example:

  .. code-block:: c

     string_t* str1 = init_string("Hello ");
     string_t* str2 = init_string("World!");
     if (str1 && str2) {
         printf("Before: %s\n", get_string(str1));
         if (string_string_concat(str1, str2)) {
             printf("After:  %s\n", get_string(str1));
         }
         free_string(str1);
         free_string(str2);
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

     string_t* str = init_string("Hello ");
     if (str) {
         printf("Before: %s\n", get_string(str));
         if (string_lit_concat(str, "World!")) {
             printf("After:  %s\n", get_string(str));
         }
         free_string(str);
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

     string_t* str1 = init_string("Hello ");
     string_t* str2 = init_string("World ");
     
     if (str1 && str2) {
         printf("Initial: %s\n", get_string(str1));
         
         // Using macro with string_t
         string_concat(str1, str2);
         printf("After string_t concat: %s\n", get_string(str1));
         
         // Using macro with literal
         string_concat(str1, "!!!");
         printf("After literal concat: %s\n", get_string(str1));
         
         free_string(str1);
         free_string(str2);
     }

  Output::

     Initial: Hello 
     After string_t concat: Hello World 
     After literal concat: Hello World !!!

String Comparison Functions
---------------------------

compare_strings_lit
^^^^^^^^^^^^^^^^^^^
.. c:function:: int compare_strings_lit(const string_t* str_struct, const char* string)

  Compares a ``string_t`` object with a C string literal lexicographically.
  Developers should consider using the :ref:`compare_strings macro <compare-strings-macro>`
  in place of the ``compare_strings_lit`` function. 

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
^^^^^^^^^^^^^^^^^^^^^^
.. c:function:: int compare_strings_string(const string_t* str_struct_one, string_t* str_struct_two)

  Compares two ``string_t`` objects lexicographically. Developers should consider 
  using the :ref:`compare-strings macro <compare-strings-macro>` in place of the 
  ``compare_strings_string`` function. 

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
^^^^^^^^^^^^^^^^^^^^^
.. c:macro:: compare_strings(str_one, str_two)

  A generic macro that automatically selects the appropriate comparison function
  based on the type of the second argument.

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

copy_string
^^^^^^^^^^^
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

     string_t* str = init_string("Hello");
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
         free_string(str);
     }

  Output::

     Initial allocation: 6
     After reserve(20): 20
     Failed to reduce allocation as expected
     String content: Hello
