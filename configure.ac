# Process this file with autoconf to produce a configure script

# prelude
AC_PREREQ([2.69])
AC_INIT([QuickXor], [0.10], [tekki@tekki.ch])
AM_PROG_AR

# unique source file --- primitive safety check 
AC_CONFIG_SRCDIR([src/quickxor.c])

# place to put some extra build scripts installed
# AC_CONFIG_AUX_DIR([build-aux])

# fairly severe build strictness
# change foreign to gnu or gnits to comply with gnu standards
AM_INIT_AUTOMAKE([-Wall -Werror 1.11.2])

# checks for programs
AC_PROG_CC
LT_INIT

# checks for libraries

# PKG_CHECK_MODULES([REQUIRED], [])
PKG_CHECK_MODULES([TEST], [check >= 0.10.0])
AM_PROG_CC_C_O

# checks for header files
AC_HEADER_STDC
AC_CHECK_HEADERS([stdlib.h string.h])

# checks for typedefs, structures, and compiler characteristics

# checks for library functions
# AC_FUNC_MALLOC

# output files
AC_CONFIG_HEADERS([config.h])

AC_CONFIG_FILES([Makefile src/Makefile tests/Makefile README.md])

AC_OUTPUT
