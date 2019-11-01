## Process this file with automake to produce Makefile.in

TESTS = t_quickxor
check_PROGRAMS = t_quickxor
t_quickxor_SOURCES = t_quickxor.c $(top_builddir)/src/quickxor.h
t_quickxor_LDADD = $(top_builddir)/src/libquickxor.la @REQUIRED_LIBS@ @TEST_LIBS@
t_quickxor_CFLAGS = @TEST_CFLAGS@
