CMAKE_MINIMUM_REQUIRED(VERSION 2.6)

PROJECT(logger)
INCLUDE_DIRECTORIES( include lib )

ADD_SUBDIRECTORY( src ${CMAKE_CURRENT_BINARY_DIR}lib )
ADD_SUBDIRECTORY( exampleProgram ${CMAKE_CURRENT_BINARY_DIR}lib )