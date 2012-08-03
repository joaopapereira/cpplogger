
CMAKE_MINIMUM_REQUIRED(VERSION 2.6)

SET( lib_srcs libJPLogger.cpp )

ADD_LIBRARY( JPLoggerStatic STATIC ${lib_srcs})
ADD_LIBRARY( JPLogger SHARED ${lib_srcs})

