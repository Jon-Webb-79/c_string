.. Core Utilities documentation master file, created by
   sphinx-quickstart
   You can adapt this file completely to your liking, but it should at least
   contain the root `toctree` directive.

Welcome to C String documentation!
==================================
C's built-in string handling, based on null-terminated character arrays, while 
efficient for simple operations, presents several challenges in modern software 
development:

* Fixed buffer sizes require precise size calculations to prevent buffer overflows
* Manual memory management leads to common pitfalls like memory leaks and dangling pointers
* No built-in bounds checking makes string operations inherently risky
* String manipulation requires careful attention to null termination
* Concatenation and resizing operations are error-prone

The c_string library addresses these challenges by providing a dynamic string 
type that:

* Automatically manages memory allocation and resizing
* Maintains string length and capacity information
* Ensures null termination
* Provides safe string manipulation functions
* Prevents direct buffer access to avoid memory corruption

When to Use This Library
########################

All of the functions in this library can be accessed from the ``c_string.h`` file.
This library is particularly useful when:

* Working with strings of unknown or varying length
* Performing frequent string concatenation operations
* Building strings incrementally
* Requiring safe string manipulation in security-sensitive contexts
* Managing multiple string operations with minimal memory overhead

The library's encapsulated design prevents common string-related bugs while 
maintaining the performance characteristics expected in C programs.

This project enapsulates its funcitonality into the ``c_string.h`` header file, 
which is wrapped in the header guard ``#ifdef __cplusplus`` which allows it to be 
compiled with C++ as well as C.  The header file name ``c_string.h`` was choosen to 
ensure it did not conflict with the ``cstring.h`` header file used in the 
C++ language.

.. toctree::
   :maxdepth: 2
   :caption: Contents:

   C Strings <String>
   String Vector <Vector>
   Dictionary <Dict>
   Generic Macros <Macros>

Indices and tables
==================

* :ref:`genindex`
* :ref:`modindex`
* :ref:`search`

Contributing
============
Pull requests are welcome.  For major changes, please open an issue first to discuss
what you would like to change.  Please make sure to include and update tests
as well as relevant doc-string and sphinx updates.

License
=======
The License is included in the **c_string** package

Requirements
============
This library is developed and tested on Macintosh and Arch Linux Operating
Systems.  It is developed with ``gcc 14.2.1`` and ``clang 16.0.6`` compilers. In
addition, this code base requires the use of ``CMake 3.31.3``, ``cmocka``, and 
``valgrind``.  This code is compiled and written with the C17 standard; however, this 
should work with any compiler using C11 or later versions.

Installation and Build Guide
############################

Requirements
------------
- Git
- CMake (version 3.31.3 or later)
- C compiler (GCC, Clang, or MSVC)

For unit testing:
- Linux: valgrind (optional, for memory leak checking)
- All platforms: cmocka testing framework

Getting the Code
----------------
Clone the repository:

.. code-block:: bash

  git clone https://github.com/Jon-Webb-79/c_string.git
  cd c_string

Debug Build (with tests)
------------------------

Use the appropriate script for your platform:

**Linux/macOS (bash)**:

.. code-block:: bash

  cd scripts/bash
  ./debug.sh

**Linux/macOS (zsh)**:

.. code-block:: bash

  cd scripts/zsh
  ./debug.zsh

**Windows**:

.. code-block:: batch

  cd scripts\Windows
  debug.bat

Run tests:

**Linux (with valgrind)**:

.. code-block:: bash

  cd build/debug
  valgrind ./unit_tests

**macOS/Windows**:

.. code-block:: bash

  cd build/debug
  ./unit_tests

Static Library Build
--------------------
Creates a static library without tests:

**Linux/macOS (bash)**:

.. code-block:: bash

  cd scripts/bash
  ./static.sh

**Linux/macOS (zsh)**:

.. code-block:: bash

  cd scripts/zsh
  ./static.zsh

**Windows**:

.. code-block:: batch

  cd scripts\Windows
  static.bat

System Installation
-------------------
Installs library files to system directories for use in other projects:

**Linux/macOS (requires sudo)**:

.. code-block:: bash

  cd scripts/bash  # or scripts/zsh
  sudo ./install.sh  # or sudo ./install.zsh

**Windows (requires Administrator)**:

1. Right-click ``scripts\Windows\install.bat``
2. Select "Run as Administrator"

Usage in Projects
-----------------
After installation, you can use the library in three ways:

1. **As System Library**:

  After installation, include in your C files:

  .. code-block:: c

     #include <c_string.h>

2. **As Static Library**:

  Link against the static library created in the build/static directory.

3. **Direct Integration**:

  Copy ``c_string.c`` and ``c_string.h`` directly into your project.

Troubleshooting
---------------
- If tests fail, ensure all dependencies are properly installed
- For Windows builds, ensure you're using an appropriate Visual Studio version
- For installation issues, verify you have appropriate system permissions

Contribute to Code Base 
-----------------------
#. Establish a pull request with the git repository owner.

#. Once the package has been downloade, you will also need to install
   Python3.10 or later version to support documentation with Sphinx.

#. Navigate to the ``c_string/docs/doxygen`` directory.

#. Create a Python virtual environment with the following command.

   .. code-block:: bash 

      python -m venv .venv 

#. Activate the virtual environment with the following command.

.. table:: Activation Commands for Virtual Environments

   +----------------------+------------------+-------------------------------------------+
   | Platform             | Shell            | Command to activate virtual environment   |
   +======================+==================+===========================================+
   | POSIX                | bash/zsh         | ``$ source <venv>/bin/activate``          |
   +                      +------------------+-------------------------------------------+
   |                      | fish             | ``$ source <venv>/bin/activate.fish``     |
   +                      +------------------+-------------------------------------------+
   |                      | csh/tcsh         | ``$ source <venv>/bin/activate.csh``      |
   +                      +------------------+-------------------------------------------+
   |                      | Powershell       | ``$ <venv>/bin/Activate.ps1``             |
   +----------------------+------------------+-------------------------------------------+
   | Windows              | cmd.exe          | ``C:\> <venv>\\Scripts\\activate.bat``    |
   +                      +------------------+-------------------------------------------+
   |                      | PowerShell       | ``PS C:\\> <venv>\\Scripts\\Activate.ps1``|
   +----------------------+------------------+-------------------------------------------+

#. Install packages to virtual environments from ``requirements.txt`` file

   .. code-block:: bash 

      pip install -r requirements.txt

#. At this point you can build the files in the same way described in the 
   previous section and contribute to documentation.
