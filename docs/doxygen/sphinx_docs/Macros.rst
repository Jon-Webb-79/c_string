******
Macros
******

Generic Macro Overview
======================

The C language's ``_Generic`` keyword, introduced in C11, enables type-safe generic programming
through compile-time type selection. This feature allows the creation of macros that can
adapt their behavior based on argument types while maintaining type safety and avoiding the
pitfalls of traditional C macros.

The macros in this library leverage ``_Generic`` to provide intuitive interfaces that work
seamlessly with both string literals (char*) and string_t objects. This type-safe approach
ensures proper function selection at compile time, reducing runtime overhead and preventing
type-related errors.

Benefits of Generic Macros
--------------------------
- Type safety: Compiler catches type mismatches at compile time
- Code clarity: Single interface for multiple types
- Performance: No runtime overhead for type checking
- Maintainability: Centralized type handling logic

String Operation Macros
-----------------------
The following macros can be used to manipulate string data.

.. _string-concat-macro:

string_concat
~~~~~~~~~~~~~
A type-safe macro for string concatenation that automatically selects the appropriate
function based on the type of the second argument:

- For char* arguments: Calls string_lit_concat
- For string_t* arguments: Calls string_string_concat

This eliminates the need for developers to manually choose between different concatenation
functions while ensuring type safety. This Macro may be safely used in place 
of the :ref:`string_string_concat() <string-string-concat-func>`
and :ref:`string_list_concat() <string-lit-concat-func>`

Example:

.. code-block:: c

  string_t* str = init_string("Hello ");
  
  // Using with string literal
  string_concat(str, "world");  // Uses string_lit_concat
  
  // Using with string_t
  string_t* other = init_string(" again");
  string_concat(str, other);    // Uses string_string_concat

.. _compare-strings-macro:

string_compare
~~~~~~~~~~~~~~
A type-safe comparison macro that selects the appropriate comparison function based on
the second argument's type:

- For char* arguments: Calls compare_strings_lit
- For string_t* arguments: Calls compare_strings_string

Provides a uniform interface for string comparison while maintaining type safety and
optimal performance. This Macro may be safely used in place of the
:ref:`compare_strings_string() <string-string-compare-func>` and
:ref:`compare_strings_lit() <string-lit-compare-func>` functions.

Example:

.. code-block:: c

  string_t* str = init_string("hello");
  
  // Compare with literal
  if (string_compare(str, "hello") == 0) {
      printf("Strings are equal\n");
  }
  
  // Compare with string_t
  string_t* other = init_string("world");
  if (string_compare(str, other) < 0) {
      printf("'hello' comes before 'world'\n");
  }

Size Inspection Macros
----------------------
These macros provide several advantages:

- **Consistency**: Same syntax for both string types
- **Type Safety**: Compiler ensures correct function selection
- **Code Clarity**: Eliminates need to remember type-specific function names
- **Maintainability**: Changes to underlying functions only need updates in one place

.. _s-size-macro:

s_size
~~~~~~
A generic macro that provides a uniform interface for checking the current populated
size of both string_t and string_v objects. It automatically selects:

- For string_t*: Uses string_size to get string length
- For string_v*: Uses str_vector_size to get number of strings

This macro simplifies size queries by providing a consistent interface regardless of
the underlying type. This Macro may be safely used in place of the 
:ref:`string_size() <string-size-func>` and :ref:`str_vector_size() <str-vector-size-func>`

Example:

.. code-block:: c

  string_t* str = init_string("hello world");
  size_t str_len = s_size(str);       // Returns 11
  
  string_v* vec = init_str_vector(2);
  push_back_str_vector(vec, "first");
  push_back_str_vector(vec, "second");
  size_t vec_len = s_size(vec);       // Returns 2

.. _s-alloc-macro:

s_alloc
~~~~~~~
A generic macro that provides a uniform interface for checking the current allocation
size of both string_t and string_v objects. It selects:

- For string_t*: Uses string_alloc to get allocated buffer size
- For string_v*: Uses str_vector_alloc to get allocated vector capacity

This macro is particularly useful for capacity planning and debugging memory usage.
This Macro may be safely used in place of the :ref:`string_alloc <string-alloc-func>`
and :ref:`str-vector-alloc() <str-vector-alloc-func>` functions.

Example:

.. code-block:: c

  string_t* str = init_string("test");
  size_t str_capacity = s_alloc(str);    // Returns 5 (includes null terminator)
  
  string_v* vec = init_str_vector(10);
  size_t vec_capacity = s_alloc(vec);    // Returns 10

Example combining both macros:

.. code-block:: c

  void print_metrics(void* data, const char* type) {
      if (strcmp(type, "string") == 0) {
          string_t* str = (string_t*)data;
          printf("String length: %zu, allocated: %zu\n",
                 s_size(str), s_alloc(str));
      } else if (strcmp(type, "vector") == 0) {
          string_v* vec = (string_v*)data;
          printf("Vector size: %zu, capacity: %zu\n",
                 s_size(vec), s_alloc(vec));
      }
  }
