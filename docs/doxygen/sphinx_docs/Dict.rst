**********
Dictionary
**********

Overview
========

The c_string library provides a hash table implementation for maintaining key-value
pairs. The dictionary implementation features automatic memory management, including
optional garbage collection when using GCC or Clang compilers.

Memory Management
-----------------

The dictionary supports automatic cleanup via the DICT_GBC macro when using 
compatible compilers:

.. code-block:: c

   void example_function(void) {
       DICT_GBC dict_t* dict = init_dict();
       // Use dictionary normally
       // No need to call free_dict - cleanup happens automatically
   }  // dict is freed here

Without garbage collection, manual cleanup is required:

.. code-block:: c

   void example_function(void) {
       dict_t* dict = init_dict();
       // Use dictionary
       free_dict(dict);  // Manual cleanup required
   }

Core Functions
==============

Initialization and Cleanup
--------------------------

init_dict
~~~~~~~~~
.. c:function:: dict_t* init_dict(void)

   Creates and initializes a new dictionary with a default capacity of 3 buckets.

   :returns: Pointer to new dictionary, or NULL on allocation failure
   :raises: Sets errno to ENOMEM if memory allocation fails

   Example:

   .. code-block:: c

      // With garbage collection
      DICT_GBC dict_t* dict = init_dict();
      if (!dict) {
          fprintf(stderr, "Dictionary initialization failed\n");
          return;
      }
      
      // Without garbage collection
      dict_t* dict = init_dict();
      if (!dict) {
          fprintf(stderr, "Dictionary initialization failed\n");
          return;
      }
      // ... use dictionary ...
      free_dict(dict);

insert_dict
~~~~~~~~~~~
.. c:function:: bool insert_dict(dict_t* dict, const char* key, size_t value)

   Inserts a new key-value pair into the dictionary. Will automatically resize
   the dictionary if the load factor exceeds the threshold.

   :param dict: Target dictionary
   :param key: String key to insert (will be copied)
   :param value: Value to associate with key
   :returns: true if successful, false if key exists or on error
   :raises: Sets errno to EINVAL for NULL inputs or duplicate key, ENOMEM for allocation failure

   Example:

   .. code-block:: c

      DICT_GBC dict_t* dict = init_dict();
      
      // Insert some values
      if (insert_dict(dict, "temperature", 98.6)) {
          printf("Temperature added\n");
      }
      
      // Trying to insert duplicate key
      if (!insert_dict(dict, "temperature", 99.1)) {
          printf("Key already exists\n");
      }

update_dict
~~~~~~~~~~~
.. c:function:: bool update_dict(dict_t* dict, char* key, size_t value)

  Updates the value associated with an existing key in the dictionary.
  Will not add the key if it doesn't exist.

  :param dict: Target dictionary
  :param key: Key whose value will be updated
  :param value: New value to store
  :returns: true if key found and updated, false if key not found or on error
  :raises: Sets errno to EINVAL if key not found or inputs invalid

  Example:

  .. code-block:: c

     DICT_GBC dict_t* dict = init_dict();
     
     // Insert initial value
     insert_dict(dict, "count", 1);
     
     // Update existing value
     if (update_dict(dict, "count", 2)) {
         printf("Value updated\n");
     }
     
     // Try to update non-existent key
     if (!update_dict(dict, "missing", 5)) {
         printf("Key not found\n");
     }

get_dict_value
~~~~~~~~~~~~~~
.. c:function:: const size_t get_dict_value(const dict_t* dict, char* key)

  Retrieves the value associated with a key without removing it from the dictionary.

  :param dict: Source dictionary
  :param key: Key to look up
  :returns: Value associated with key, or LONG_MAX if not found
  :raises: Sets errno to EINVAL if inputs invalid

  Example:

  .. code-block:: c

     DICT_GBC dict_t* dict = init_dict();
     insert_dict(dict, "pi", 3.14159);
     
     size_t value = get_dict_value(dict, "pi");
     if (value != LONG_MAX) {
         printf("Pi = %f\n", value);
     }
     
     // Looking up non-existent key
     if (get_dict_value(dict, "e") == LONG_MAX) {
         printf("Key 'e' not found\n");
     }

pop_dict
~~~~~~~~
.. c:function:: size_t pop_dict(dict_t* dict, char* key)

  Removes a key-value pair from the dictionary and returns the value.
  The key and its associated memory are freed.

  :param dict: Target dictionary
  :param key: Key to remove
  :returns: Value associated with removed key, or LONG_MAX if not found
  :raises: Sets errno to EINVAL if inputs invalid

  Example:

  .. code-block:: c

     DICT_GBC dict_t* dict = init_dict();
     insert_dict(dict, "temp", 72.5);
     
     // Remove and get value
     size_t value = pop_dict(dict, "temp");
     if (value != LONG_MAX) {
         printf("Popped value: %f\n", value);
     }
     
     // Key no longer exists
     if (get_dict_value(dict, "temp") == LONG_MAX) {
         printf("Key was successfully removed\n");
     }

get_dict_keys
~~~~~~~~~~~~~
.. c:function:: string_v* get_dict_keys(const dict_t* dict)

   Creates a string vector containing all keys from the dictionary.
   The order of keys in the vector is not guaranteed.

   :param dict: Source dictionary
   :returns: New string vector containing all dictionary keys, or NULL on error
   :raises: Sets errno to EINVAL if dict is NULL, ENOMEM on allocation failure

   Example:

   .. code-block:: c

      dict_t* dict = init_dict();
      insert_dict(dict, "name", 1.0);
      insert_dict(dict, "age", 2.0);
      insert_dict(dict, "city", 3.0);
      
      STRVEC_GBC string_v* keys = get_dict_keys(dict);
      if (keys) {
          printf("Dictionary keys:\n");
          for (size_t i = 0; i < str_vector_size(keys); i++) {
              printf("%s\n", get_string(str_vector_index(keys, i)));
          }
      }
      
      free_dict(dict);

   Output::

      Dictionary keys:
      name
      age
      city

.. note::

      The returned string vector must be freed using free_str_vector()
      or can be automatically freed using the STRVEC_GBC macro.

Dictionary Information Functions
--------------------------------

.. _dict-size-func:

dict_size
~~~~~~~~~
.. c:function:: const size_t dict_size(const dict_t* dict)

  Returns the number of key-value pairs currently stored in the dictionary.
  The developer can also use the :ref:`s_size <s-size-macro>` macro in
  place of this function.

  :param dict: Dictionary to query
  :returns: Number of key-value pairs, or LONG_MAX on error
  :raises: Sets errno to EINVAL if dict is NULL

  Example:

  .. code-block:: c

     DICT_GBC dict_t* dict = init_dict();
     insert_dict(dict, "one", 1);
     insert_dict(dict, "two", 2);
     
     printf("Dictionary contains %zu items\n", dict_size(dict));

.. _dict-alloc-func:

dict_alloc
~~~~~~~~~~
.. c:function:: const size_t dict_alloc(const dict_t* dict)

  Returns the current allocation size (number of buckets) in the dictionary.
  The developer can also use the :ref:`s_alloc <s-alloc-macro>` macro in
  place of this function.

  :param dict: Dictionary to query
  :returns: Current bucket count, or LONG_MAX on error
  :raises: Sets errno to EINVAL if dict is NULL

  Example:

  .. code-block:: c

     DICT_GBC dict_t* dict = init_dict();
     printf("Initial bucket count: %zu\n", dict_alloc(dict));
     
     // Add items until resize occurs
     for (int i = 0; i < 10; i++) {
         char key[10];
         sprintf(key, "key%d", i);
         insert_dict(dict, key, i);
         printf("Bucket count: %zu\n", dict_alloc(dict));
     }

dict_hash_size
~~~~~~~~~~~~~~
.. c:function:: const size_t dict_hash_size(const dict_t* dict)

  Returns the total number of hash entries in the dictionary.
  This may differ from dict_size() due to hash collisions.

  :param dict: Dictionary to query
  :returns: Number of hash entries, or LONG_MAX on error
  :raises: Sets errno to EINVAL if dict is NULL

is_key_value
~~~~~~~~~~~~
.. c:function:: bool is_key_value(const dict_t* dict, const char* key)

  Checks if a key exists in the dictionary.

  :param dict: Dictionary to search
  :param key: Key to look for
  :returns: true if key exists, false if not found or on error
  :raises: Sets errno to EINVAL if inputs invalid

  Example:

  .. code-block:: c

     DICT_GBC dict_t* dict = init_dict();
     insert_dict(dict, "exists", 1);
     
     if (is_key_value(dict, "exists")) {
         printf("Key found\n");
     }
     
     if (!is_key_value(dict, "missing")) {
         printf("Key not found\n");
     }
