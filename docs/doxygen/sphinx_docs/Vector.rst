******
Vector
******

String Vector Overview
=======================

A string vector provides a dynamic array of string_t objects, offering efficient management of
multiple strings with automatic memory handling and resizing capabilities. This implementation
combines the flexibility of dynamic arrays with the safety and convenience of the string_t type.

Key Features
------------

* Dynamic resizing: Automatically grows to accommodate new strings
* Memory safety: Proper encapsulation and memory management
* String type integration: Direct compatibility with string_t objects
* Efficient access: O(1) access to any string in the vector
* Automatic cleanup: Optional garbage collection support with STRVEC_GBC

When to Use String Vectors
--------------------------

String vectors are particularly useful when:

* Managing collections of strings that may grow or shrink
* Working with an unknown number of strings at compile time
* Needing sequential access to multiple strings
* Requiring efficient random access to strings by index
* Managing related strings as a single unit
* Implementing string-based data structures like command histories or word lists

Performance Characteristics
---------------------------

* Access time: O(1) for index-based access
* Insertion time: O(1) amortized for push_back operations
* Memory efficiency: Grows geometrically to minimize reallocations
* Memory overhead: Minimal per-string overhead beyond string content

Vector Functions 
================
The c_string library provides a dynamically allocated string vector of ``string_t``
data types.  All of the functions described in this section can be found in the 
``c_string.h`` header file.

Data Types 
----------

string_v
~~~~~~~~
The dynamic string vector type that manages memory allocation automatically and 
provides a safe encapsulation for ``string_t`` data types in an array format.
This struct is provided as an opaque data structure.

.. code-block:: c

   typedef struct {
       string_t* data;
       size_t len;
       size_t alloc;
   } string_v;

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
    } string_t;

Core Functions 
--------------
The functions and Macros in this section are core to the creation and destruction of
``string_v`` data types.

Initialization and Memory Management
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
The functions and Macros in this section are used to control the creation,
memory allocation, and specific destruction of ``string_v`` data types.

init_str_vector
^^^^^^^^^^^^^^^
.. c:function:: string_v* init_str_vector(size_t buffer)

  Initializes a new string vector with specified initial capacity.
  The vector will automatically grow if needed when adding elements.

  :param buffer: Initial capacity (number of strings) to allocate
  :returns: Pointer to new ``string_v`` object, or NULL on allocation failure
  :raises: Sets errno to ENOMEM if memory allocation fails

  Example:

  .. code-block:: c

     // Create vector with initial capacity for 10 strings
     string_v* vec = init_str_vector(10);
     if (!vec) {
         fprintf(stderr, "Failed to initialize vector\n");
         return 1;
     }

     // Use the vector
     push_back_str_vector(vec, "hello");
     
     // Free when done
     free_str_vector(vec);

push_back_str_vector
^^^^^^^^^^^^^^^^^^^^
.. c:function:: bool push_back_str_vector(string_v* vec, const char* str)

  Adds a string to the end of the vector. If needed, the vector automatically
  resizes to accommodate the new string. For vectors smaller than VEC_THRESHOLD,
  capacity doubles when full. For larger vectors, a fixed amount is added.

  :param vec: Target string vector
  :param str: String to add to vector
  :returns: true if successful, false on error
  :raises: Sets errno to EINVAL for NULL inputs or ENOMEM on allocation failure

  Example:

  .. code-block:: c

     string_v* vec = init_str_vector(2);
     
     // Add some strings
     push_back_str_vector(vec, "first");
     push_back_str_vector(vec, "second");
     
     // Vector will automatically resize
     push_back_str_vector(vec, "third");
     
     printf("Vector size: %zu\n", str_vector_size(vec));
     
     free_str_vector(vec);

  Output::

     Vector size: 3

free_str_vector
^^^^^^^^^^^^^^^
.. c:function:: void free_str_vector(string_v* vec)

  Frees all memory associated with a string vector, including all contained
  strings. After calling, the vector pointer should not be used.  If using a
  ``gcc`` or ``clang`` compiler, the developer should consider using the 
  :ref:`STRVEC_GBC <stringv_cleanup_macro>` Macro in place of the ``free_str_vector`` 
  function.

  :param vec: String vector to free
  :raises: Sets errno to EINVAL if vec is NULL

  Example:

  .. code-block:: c

     string_v* vec = init_str_vector(5);
     push_back_str_vector(vec, "hello");
     push_back_str_vector(vec, "world");
     
     // Free the vector and all its strings
     free_str_vector(vec);
     vec = NULL;  // Good practice to avoid dangling pointer

Automatic Cleanup 
~~~~~~~~~~~~~~~~~
In general the C language does not allow automated garbage collection of 
memory that is out of scope.  This section describes a poor mans 
garbage collection within the C language, for the ``string_v`` data type,
that can only be enabled if compiled with ``gcc`` or ``clang``.

.. _stringv_cleanup_macro:

STRVEC_GBC
^^^^^^^^^^

.. c:macro:: STRVEC_GBC

  Macro that enables automatic cleanup of string vectors when they go out of
  scope. Only available when using GCC or Clang compilers. Uses the cleanup
  attribute to automatically call _free_str_vector.

  Example:

  .. code-block:: c

     void process_strings(void) {
         // Vector will be automatically freed when function returns
         STRVEC_GBC string_v* vec = init_str_vector(10);
         
         push_back_str_vector(vec, "hello");
         push_back_str_vector(vec, "world");
         
         // No need to call free_str_vector
     }  // vec is 
