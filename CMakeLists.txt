CMAKE_MINIMUM_REQUIRED(VERSION 2.6)

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


SET(LIBRARY_OUTPUT_PATH .)
SET(EXECUTABLE_OUTPUT_PATH .)

PROJECT(logger)
INCLUDE_DIRECTORIES( include lib )

ADD_SUBDIRECTORY( src lib )

INSTALL( FILES include/libJPLogger.h
         DESTINATION ${INSTALL_INCS} )

INSTALL( FILES lib/liblogger.so
         DESTINATION ${INSTALL_LIBS} )
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
  )

  ADD_CUSTOM_COMMAND(
    DEPENDS clean
    COMMENT "distribution clean"
    COMMAND rm
    ARGS    -Rf CMakeTmp ${DISTCLEANED}
    TARGET  distclean
  )
ENDIF( UNIX )

