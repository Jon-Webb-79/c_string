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

Data Types 
==========
The following are derived data types used in the implementation for a string 
vector.

string_v
--------
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
--------
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
==============
The c_string library provides a dynamically allocated string vector of ``string_t``
data types.  All of the functions described in this section can be found in the 
``c_string.h`` header file.

Initialization and Memory Management
------------------------------------
The functions and Macros in this section are used to control the creation,
memory allocation, and specific destruction of ``string_v`` data types.

The functions and Macros in this section are core to the creation and destruction of
``string_v`` data types.

.. warning::

   The ``string_v`` methods encapsulates ``string_t`` data types into a dynamically 
   allocated vector.  The user should take care not to free data within the 
   vector with the :ref:`free_string() <free-string-func>` function or the 
   :ref:`STRING_GBC <string_cleanup_macro>` macro to avoid dangling pointers

init_str_vector
~~~~~~~~~~~~~~~
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

free_str_vector
~~~~~~~~~~~~~~~
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

String Data Addition
--------------------
The following functions provide methods for adding string data to a ``string_v``
data type.

push_back_str_vector
~~~~~~~~~~~~~~~~~~~~
.. c:function:: bool push_back_str_vector(string_v* vec, const char* str)

  Adds a string to the end of the vector. If needed, the vector automatically
  resizes to accommodate the new string. For vectors smaller than VEC_THRESHOLD,
  capacity doubles when full. For larger vectors, a fixed amount is added.
  This is the most efficient method for adding data to a string vector with 
  a time efficiency of :math:`O(1)`.

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

  This method can also be used indirectly with existing ``string_t`` data types as shown 
  by the example below.

  .. code-block:: c

     string_v* vec = init_str_vector(2);
     
     // Create Strings 
     string_t* str1 STRING_GBC = init_string("first");
     string_t* str2 STRING_GBC = init_string("second");
     // Add some strings
     push_back_str_vector(vec, get_string(str1));
     push_back_str_vector(vec, get_string(str2));
     
     // Vector will automatically resize
     push_back_str_vector(vec, "third");
     
     printf("Vector size: %zu\n", str_vector_size(vec));
     
     free_str_vector(vec);

    Output::

     Vector size: 3

  The folllowing should be considered when using this function

  * If reallocation fails, the original vector remains unchanged
  * If string allocation fails after moving elements, the vector is restored to its original state
  * The operation requires enough contiguous memory for the entire resized array

push_front_str_vector
~~~~~~~~~~~~~~~~~~~~~
.. c:function:: bool push_front_str_vector(string_v* vec, const char* value)

  Adds a string to the beginning of the vector, shifting all existing elements
  to the right. Automatically resizes the vector if needed.
  This is the least efficient method for adding data to a string vector with 
  a time efficiency of :math:`O(n)`. 

  :param vec: Target string vector
  :param value: String to add at front
  :returns: true if successful, false on error
  :raises: Sets errno to EINVAL for NULL inputs or ENOMEM on allocation failure

  Example:

  .. code-block:: c

     string_v* vec STRVEC_GBC = init_str_vector(2);
     
     // Add "world" at the back
     push_back_str_vector(vec, "world");
     
     // Add "hello" at the front
     if (push_front_str_vector(vec, "hello")) {
         // Print all strings
         for (size_t i = 0; i < str_vector_size(vec); i++) {
             printf("%s ", get_string(str_vector_index(vec, i)));
         }
         printf("\n");
     }
     
  Output::

     hello world

  This method can also work indirectly with existing ``string_t`` data types 
  as shown below.

  .. code-block:: c

     string_v* vec STRVEC_GBC = init_str_vector(2);
     
     // Add "world" at the back
     push_back_str_vector(vec, "world");

     string_t* str STRING_GBC = init_string("hello");

     // Add "hello" at the front
     if (push_front_str_vector(vec, get_string(str))) {
         // Print all strings
         for (size_t i = 0; i < str_vector_size(vec); i++) {
             printf("%s ", get_string(str_vector_index(vec, i)));
         }
         printf("\n");
     }

  Output::

     hello world

  The folllowing should be considered when using this function

  * If reallocation fails, the original vector remains unchanged
  * If string allocation fails after moving elements, the vector is restored to its original state
  * The operation requires enough contiguous memory for the entire resized array

  .. note::

     When resizing is needed, the vector grows either by doubling (when size < VEC_THRESHOLD)
     or by adding a fixed amount (when size >= VEC_THRESHOLD). This provides efficient
     amortized performance for both small and large vectors.

insert_str_vector
~~~~~~~~~~~~~~~~~
.. c:function:: bool insert_str_vector(string_v* vec, const char* str, size_t index)

  Inserts a string at any valid position in the vector, shifting subsequent
  elements to the right. Automatically resizes the vector if needed.
  The time complexity of this function can range from :math:`O(1)` to
  :math:`O(n)` depending on where the data is inserted.

  :param vec: Target string vector
  :param str: String to insert
  :param index: Position at which to insert (0 to vec->len)
  :returns: true if successful, false on error
  :raises: Sets errno to EINVAL for NULL inputs, ERANGE for invalid index,
          or ENOMEM on allocation failure

  Example:

  .. code-block:: c

     string_v* vec STRVEC_GBC = init_str_vector(3);
     
     // Create initial vector
     push_back_str_vector(vec, "first");
     push_back_str_vector(vec, "third");
     
     // Insert "second" between them
     if (insert_str_vector(vec, "second", 1)) {
         // Print all strings
         for (size_t i = 0; i < str_vector_size(vec); i++) {
             printf("%s ", get_string(str_vector_index(vec, i)));
         }
         printf("\n");
     }

  Output::

     first second third

  The folllowing should be considered when using this function

  * If reallocation fails, the original vector remains unchanged
  * If string allocation fails after moving elements, the vector is restored to its original state
  * The operation requires enough contiguous memory for the entire resized array

  .. note::

     When resizing is needed, the vector grows either by doubling (when size < VEC_THRESHOLD)
     or by adding a fixed amount (when size >= VEC_THRESHOLD). This provides efficient
     amortized performance for both small and large vectors.

Drop String Functions
---------------------
The string vector provides three different methods for removing elements, each with
different performance characteristics.

pop_back_str_vector
~~~~~~~~~~~~~~~~~~~
.. c:function:: string_t* pop_back_str_vector(string_v* vec)

  Removes and returns the last element from the vector. This is the most efficient
  removal operation as it requires no element shifting.

  :param vec: Target string vector
  :returns: Pointer to removed string_t object, or NULL on error
  :raises: Sets errno to EINVAL for NULL input or empty vector
  :time complexity: O(1) - Constant time operation

  Example:

  .. code-block:: c

     string_v* vec = init_str_vector(2);
     push_back_str_vector(vec, "first");
     push_back_str_vector(vec, "second");
     
     string_t* popped = pop_back_str_vector(vec);
     if (popped) {
         printf("Popped string: %s\n", get_string(popped));
         printf("Remaining size: %zu\n", str_vector_size(vec));
         free_string(popped);
     }
     
     free_str_vector(vec);

  Output::

     Popped string: second
     Remaining size: 1

pop_front_str_vector
~~~~~~~~~~~~~~~~~~~~
.. c:function:: string_t* pop_front_str_vector(string_v* vec)

  Removes and returns the first element from the vector. Requires shifting all
  remaining elements left by one position.

  :param vec: Target string vector
  :returns: Pointer to removed string_t object, or NULL on error
  :raises: Sets errno to EINVAL for NULL input or empty vector
  :time complexity: O(n) - Linear time based on vector size

  Example:

  .. code-block:: c

     string_v* vec = init_str_vector(2);
     push_back_str_vector(vec, "first");
     push_back_str_vector(vec, "second");
     
     string_t* popped = pop_front_str_vector(vec);
     if (popped) {
         printf("Popped string: %s\n", get_string(popped));
         printf("New first element: %s\n", 
                get_string(str_vector_index(vec, 0)));
         free_string(popped);
     }
     
     free_str_vector(vec);

  Output::

     Popped string: first
     New first element: second

pop_any_str_vector
~~~~~~~~~~~~~~~~~~
.. c:function:: string_t* pop_any_str_vector(string_v* vec, size_t index)

  Removes and returns the element at the specified index. Performance varies based
  on the removal position - removing from the end is fast, while removing from
  the start or middle requires shifting elements.

  :param vec: Target string vector
  :param index: Position of element to remove
  :returns: Pointer to removed string_t object, or NULL on error
  :raises: Sets errno to EINVAL for NULL input or empty vector, ERANGE for invalid index
  :time complexity: O(1) to O(n) depending on index position

  Example:

  .. code-block:: c

     string_v* vec = init_str_vector(3);
     push_back_str_vector(vec, "first");
     push_back_str_vector(vec, "second");
     push_back_str_vector(vec, "third");
     
     // Remove middle element
     string_t* popped = pop_any_str_vector(vec, 1);
     if (popped) {
         printf("Popped string: %s\n", get_string(popped));
         printf("Remaining strings: %s, %s\n",
                get_string(str_vector_index(vec, 0)),
                get_string(str_vector_index(vec, 1)));
         free_string(popped);
     }
     
     free_str_vector(vec);

  Output::

     Popped string: second
     Remaining strings: first, third

.. note::

  All pop operations maintain vector consistency and properly manage memory of
  removed elements. The caller is responsible for freeing the returned string_t
  object using free_string().

Automatic Cleanup 
-----------------
In general the C language does not allow automated garbage collection of 
memory that is out of scope.  This section describes a poor mans 
garbage collection within the C language, for the ``string_v`` data type,
that can only be enabled if compiled with ``gcc`` or ``clang``.

.. _stringv_cleanup_macro:

STRVEC_GBC
~~~~~~~~~~

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

Utility Functions
=================
The following functions can be used to retrieve basic information from 
a ``string_v`` data type.

Vector Size and Access Functions 
--------------------------------
The following functions are used to determine the allocated size and populated 
length of a ``string_v`` data type/

.. _str-vector-index-func:

str_vector_index
~~~~~~~~~~~~~~~~
.. c:function:: const string_t* str_vector_index(const string_v* vec, size_t index)

  Retrieves a pointer to the string_t object at the specified index in the vector.
  Provides safe, bounds-checked access to vector elements.

  :param vec: Source string vector
  :param index: Zero-based index of desired element
  :returns: Pointer to string_t at specified index, or NULL on error
  :raises: Sets errno to EINVAL for NULL input, ERANGE for out-of-bounds index

  Example:

  .. code-block:: c

     string_v* vec = init_str_vector(2);
     push_back_str_vector(vec, "hello");
     push_back_str_vector(vec, "world");
     
     const string_t* str = str_vector_index(vec, 1);
     if (str) {
         printf("Second string: %s\n", get_string(str));
     }
     
     free_str_vector(vec);

  Output::

     Second string: world

.. _str-vector-size-func:

str_vector_size
~~~~~~~~~~~~~~~
.. c:function:: const size_t str_vector_size(const string_v* vec)

  Returns the current number of elements in the vector. This represents the
  actual number of strings stored, not the allocated capacity.

  :param vec: String vector to query
  :returns: Number of elements in vector, or LONG_MAX on error
  :raises: Sets errno to EINVAL for NULL input

  Example:

  .. code-block:: c

     string_v* vec = init_str_vector(5);  // Capacity of 5
     push_back_str_vector(vec, "first");
     push_back_str_vector(vec, "second");
     
     printf("Vector size: %zu\n", str_vector_size(vec));
     
     free_str_vector(vec);

  Output::

     Vector size: 2

The developer may also consider the safe use of the :ref:`s_alloc <s-size-macro>`
Macro in place of the ``str_vector_size`` function.

.. _str-vector-alloc-func:

str_vector_alloc
~~~~~~~~~~~~~~~~
.. c:function:: const size_t str_vector_alloc(const string_v* vec)

  Returns the current allocation size (capacity) of the vector. This represents
  the number of elements that can be stored without requiring reallocation.

  :param vec: String vector to query
  :returns: Current allocation size, or LONG_MAX on error
  :raises: Sets errno to EINVAL for NULL input

  Example:

  .. code-block:: c

     string_v* vec = init_str_vector(5);
     
     printf("Initial capacity: %zu\n", str_vector_alloc(vec));
     
     // Add strings until reallocation occurs
     for(int i = 0; i < 6; i++) {
         push_back_str_vector(vec, "test");
         printf("After push %d - Size: %zu, Capacity: %zu\n",
                i + 1, str_vector_size(vec), str_vector_alloc(vec));
     }
     
     free_str_vector(vec);

  Output::

     Initial capacity: 5
     After push 1 - Size: 1, Capacity: 5
     After push 2 - Size: 2, Capacity: 5
     After push 3 - Size: 3, Capacity: 5
     After push 4 - Size: 4, Capacity: 5
     After push 5 - Size: 5, Capacity: 5
     After push 6 - Size: 6, Capacity: 10

The developer may also consider the safe use of the :ref:`s_alloc <s-alloc-macro>`
Macro in place of the ``str_vector_alloc`` function.

.. note::

  These functions provide the basic mechanisms for inspecting a vector's state
  and accessing its contents. They are fundamental to safe vector manipulation
  and are used extensively by other vector operations.
