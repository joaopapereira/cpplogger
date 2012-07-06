
CMAKE_MINIMUM_REQUIRED(VERSION 2.6)

SET( lib_srcs libJPLogger.cpp )

ADD_LIBRARY( logger SHARED ${lib_srcs})

install (TARGETS logger DESTINATION lib/)
install (FILES include/*.h DESTINATION include/)
