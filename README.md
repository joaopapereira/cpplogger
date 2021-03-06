Logger class
=========

https://github.com/joaopapereira/cpplogger.git  [![Build Status](https://travis-ci.org/joaopapereira/cpplogger.svg?branch=master)](https://travis-ci.org/joaopapereira/cpplogger)

Overview
=========
Library in C++11 to implement a logger

With this library it is possible to implement two 
types of loggers.
One individual logger and a Singleton Logger for 
all project.
It is also possible to use both of them in the 
same application.


Requirements for End Users
=========

This library do not need any special library installed.
Uses the base c/c++ functions to implement the logger
This implementation support uses c++11. If C++11 compiler is not present it uses Boost as an alternative.


Building the library
=========
Logger comes with a CMake build script (CMakeLists.txt) that can
be used on a wide range of platforms ("C" stands for cross-platofrm.).
If you don't have CMake installed already, you can download it for
free from http://www.cmake.org/.

CMake works by generating native makefiles or build projects that can
be used in the compiler environment of your choice.  The typical
workflow starts with:

  mkdir mybuild       # Create a directory to hold the build output.
  cd mybuild
  cmake ${LOGGER_DIR}  # Generate native build scripts.

If you want to build 's samples, you should replace the
last command with

  cmake -Dlogger_build_samples=ON ${LOGGER_DIR}

If you are on a *nix system, you should now see a Makefile in the
current directory.  Just type 'make' to build logger.


If you want to build 's library with debug, you should replace the
command with

  cmake -Dcompile_with_debug=ON ${LOGGER_DIR}

If you want to build the library with Boost instead of c++11 support 
you should replace the command with

  cmake -Dforce_boost=ON ${LOGGER_DIR}
