CMAKE_MINIMUM_REQUIRED(VERSION 2.6)
#####################################
## Option part
#####################################
option(logger_build_samples "Build logger sample programs." OFF)
option(compile_with_debug "Build library with debug." OFF)
option(force_boost "Force BOOST usage instead of C++11." OFF)

#####################################
## Definition of environment
#####################################
IF( DEFINED ENV{INSTALL_LIB_DIR} )
   SET(INSTALL_LIBS $ENV{INSTALL_LIB_DIR}/ )
ELSE()
   SET(INSTALL_LIBS ${CMAKE_HOME_DIRECTORY}/lib )
ENDIF()
IF( DEFINED ENV{INSTALL_INC_DIR} )
   SET(INSTALL_INCS $ENV{INSTALL_INC_DIR}/ )
ELSE()
   SET(INSTALL_INCS ${CMAKE_HOME_DIRECTORY}/include )
ENDIF()


IF( EXISTS ${INSTALL_LIBS}/liblogger.so )
	return()
ENDIF()


SET(LIBRARY_OUTPUT_PATH .)
SET(EXECUTABLE_OUTPUT_PATH .)


#####################################
## Project name
#####################################
PROJECT(logger)

#####################################
## Include directories 
#####################################
INCLUDE_DIRECTORIES( include lib )
find_package (Threads REQUIRED)
if ("${CMAKE_CXX_COMPILER_ID}" MATCHES "GNU" AND NOT force_boost)
    execute_process(
        COMMAND ${CMAKE_CXX_COMPILER} -dumpversion OUTPUT_VARIABLE GCC_VERSION)
    if (NOT (GCC_VERSION VERSION_GREATER 4.7 OR GCC_VERSION VERSION_EQUAL 4.7))
        message("GCC Without c++11")
        FIND_PACKAGE( Boost 1.40 COMPONENTS program_options REQUIRED )
	INCLUDE_DIRECTORIES( ${Boost_INCLUDE_DIR} )
        ADD_DEFINITIONS( -DUSE_BOOST_INSTEAD_CXX11 ${CMAKE_THREAD_LIBS_INIT} )
    	SET(ADDITIONAL_LINK_LIBS boost_system)
    else()
        message("GCC With c++11")
        ADD_DEFINITIONS( -std=c++11 ${CMAKE_THREAD_LIBS_INIT} )
    	SET(ADDITIONAL_LINK_LIBS )
    endif ()
else ()
    message("Force Boost")
    FIND_PACKAGE( Boost 1.40 COMPONENTS program_options REQUIRED )
    INCLUDE_DIRECTORIES( ${Boost_INCLUDE_DIR} )
    ADD_DEFINITIONS( -DUSE_BOOST_INSTEAD_CXX11 ${CMAKE_THREAD_LIBS_INIT} )
    SET(ADDITIONAL_LINK_LIBS boost_system)
endif ()

#####################################
## Folders to be build
#####################################
ADD_SUBDIRECTORY( src lib )
#####################################
## Samples to be build
#####################################
if( logger_build_samples)
	ADD_SUBDIRECTORY( exampleProgram bin )
endif()



#####################################
# Search for needed lib
#####################################
#####################################
# Instalation section
#####################################
INSTALL( FILES include/libJPLogger.hpp
         DESTINATION ${INSTALL_INCS} )

INSTALL( FILES lib/libJPLoggerStatic.a
         DESTINATION ${INSTALL_LIBS} )

INSTALL( FILES lib/libJPLogger.so
         DESTINATION ${INSTALL_LIBS} )

#####################################
# Others
#####################################
IF( UNIX )
   ADD_CUSTOM_TARGET( distclean @@echo cleaning for source distribution)
  SET(DISTCLEANED
   cmake.depends
   cmake.check_depends
   CMakeCache.txt
   cmake.check_cache
   *.cmake
   Makefile
   core core.*
   gmon.out
   *~
   lib
   CMakeFiles
   bin
   install_*
   docs/docs/
   docs/logger.dox
  )

  ADD_CUSTOM_COMMAND(
    DEPENDS clean
    COMMENT "distribution clean"
    COMMAND rm
    ARGS    -Rf CMakeTmp ${DISTCLEANED}
    TARGET  distclean
  )
ENDIF( UNIX )
if( compile_with_debug )
	set(CMAKE_CXX_FLAGS "-DDEBUG=1")
	set(CMAKE_BUILD_TYPE Debug)
endif()

#####################################
# add a target to generate API documentation with Doxygen
#####################################
find_package(Doxygen)
if(DOXYGEN_FOUND)
	configure_file(${CMAKE_CURRENT_SOURCE_DIR}/docs/logger.dox.in ${CMAKE_CURRENT_BINARY_DIR}/docs/logger.dox @ONLY)
	add_custom_target(docs
			${DOXYGEN_EXECUTABLE} ${CMAKE_CURRENT_BINARY_DIR}/docs/logger.dox
			WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
			COMMENT "Generating Logger documentation with Doxygen" VERBATIM
)
endif(DOXYGEN_FOUND)
