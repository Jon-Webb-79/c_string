********
c_string
********

C's built-in string handling, based on null-terminated character arrays, while efficient for simple operations, presents several challenges in modern software development:

* Fixed buffer sizes require precise size calculations to prevent buffer overflows
* Manual memory management leads to common pitfalls like memory leaks and dangling pointers
* No built-in bounds checking makes string operations inherently risky
* String manipulation requires careful attention to null termination
* Concatenation and resizing operations are error-prone

The c_string library addresses these challenges by providing a dynamic string type that:

* Automatically manages memory allocation and resizing
* Maintains string length and capacity information
* Ensures null termination
* Provides safe string manipulation functions
* Prevents direct buffer access to avoid memory corruption

When to Use This Library
-----------------------

This library is particularly useful when:

* Working with strings of unknown or varying length
* Performing frequent string concatenation operations
* Building strings incrementally
* Requiring safe string manipulation in security-sensitive contexts
* Managing multiple string operations with minimal memory overhead

The library's encapsulated design prevents common string-related bugs while maintaining the performance characteristics expected in C programs.

Contributing
############
Pull requests are welcome.  For major changes, please open an issue first to discuss
what you would like to change.  Please make sure to include and update tests
as well as relevant doc-string and sphinx updates.

License
#######
The License is included in the **project_name** package

Requirements
############
This library is developed and tested on Macintosh and Arch Linux Operating
Systems.  It is developed with ``gcc 14.2.1`` and ``clang 16.0.6`` compilers. In
addition, this code base requires the use of ``CMake 3.31.3``, ``cmocka``, and 
``valgrind``.  This code is compiled and written with the C17 standard; however, this 
should work with any compiler using C11 or later versions.

Installation
############
In order to download this repository from github, follow these instructions

#. Ensure you have ``.git`` installed on your computer

#. Ensure you have ``cmake`` installed on your computer.  This code-base requires 
   cmake version 3.31.3 or later versions.

#. Download this repository to your preferred directory with the following command;

   .. code-block:: bash 

      git clone https://github.com/Jon-Webb-79/c_string.git c_string 

#. Navigate to either the bash or zshell scripts directory depending on your 
   environment with one of the following commands.

   .. code-block:: bash 

      cd c_string/scripts/bash 
      cd c_string/scripts/zsh 

#. When developing code, build the code base with the following command.

   .. code-block:: bash 

      # If using bash
      bash debug.sh  
      # If using zsh 
      zsh debug.zsh

#. Navigate to ``csalt/csalt/build/debug`` to run unit tests 

   .. code-block:: bash 

      valgrind ./unit_tests 
      
#. If all unit tests do not pass with no memory leaks, you may need to contact 
   the administrator of this git repository.  If they do pass, then you are set 
   to transform this code-base into a static or dynamic library, or just 
   copy the ``.c`` and ``.h`` files to your project.

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

Documentation 
=============
This code in this repository is further documented at the XXX
website
