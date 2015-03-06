#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.69])
AC_INIT(min-max-heap, 0.1)
AC_CONFIG_SRCDIR([src/radix_tree.c])
AC_CONFIG_HEADERS([config.h])
LT_INIT
AC_CONFIG_MACRO_DIRS([m4])

# Checks for programs.
AC_PROG_CC_STDC
AC_PROG_INSTALL
AC_PROG_MAKE_SET

# Checks for libraries.
# FIXME: Replace `main' with a function in `-lm':
AC_CHECK_LIB([m], [log2])

AM_INIT_AUTOMAKE([subdir-objects])

# Checks for header files.
AC_CHECK_HEADERS([stdint.h stdlib.h string.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_CHECK_HEADER_STDBOOL
AC_TYPE_UINT32_T

# Checks for library functions.
AC_FUNC_REALLOC
AC_CHECK_FUNCS([floor])

AC_CONFIG_FILES([Makefile])
AC_OUTPUT

