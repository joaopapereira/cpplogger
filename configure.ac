dnl Process this file with autoconf to produce a configure script.

AC_PREREQ(2.63)
AC_INIT([logger],[1.0])
AC_PROG_CXX

AC_CANONICAL_TARGET

dnl Initialize automake
AM_INIT_AUTOMAKE

dnl this allows us specify individual liking flags for each target
AM_PROG_CC_C_O 

dnl Initialize Libtool
LT_INIT

dnl Check if Libtool is present
dnl Libtool is used for building share libraries 
LT_INIT

AC_CONFIG_FILES(Makefile
                exampleProgram/Makefile
                libJPLogger/Makefile
                include/Makefile)
AC_OUTPUT
