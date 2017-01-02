#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.69])
AC_INIT(GrylloEngine, 0.1.1, hakeris1010@gmail.com)
AC_CONFIG_SRCDIR([examples/entity.cpp])
AC_CONFIG_HEADERS([config.h])

# Checks for programs.
AC_PROG_CXX
AC_PROG_CC

# Checks for libraries.
# - AC_SEARCH_LIBS(function, libraries-list, action-if-found, action-if-not-found, extra-libraries)

# LIBS_COMMON = 
# LIBS_WIND = 

AC_SEARCH_LIBS([], [sfml-graphics-s sfml-window-s sfml-system-s sfml-audio-s jpeg opengl32 glu32 winmm], [], [AC_MSG_ERROR([unable to find the needed libraries.])])

# Checks for header files.
# - AC_CHECK_HEADERS([headList])
AC_CHECK_HEADERS([SFML/Graphics.hpp])

# Checks for typedefs, structures, and compiler characteristics.
AC_CHECK_HEADER_STDBOOL
AC_C_INLINE
AC_TYPE_SIZE_T
AC_TYPE_UINT32_T

# Checks for library functions.

AC_OUTPUT

AM_INIT_AUTOMAKE(GrylloEngine, 2.6)