## Process this file with automake to produce Makefile.in

lib_LTLIBRARIES = libquickxor.la
libquickxor_la_SOURCES = quickxor.c quickxor.h
include_HEADERS = quickxor.h

bin_PROGRAMS = quickxorhash
quickxorhash_SOURCES = quickxor_bin.c
quickxorhash_LDADD = libquickxor.la @REQUIRED_LIBS@
