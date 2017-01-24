#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.69])
AC_INIT(GrylloEngine, 0.1.1, hakeris1010@gmail.com)

AM_INIT_AUTOMAKE(GrylloEngine, 2.6)

AC_CONFIG_SRCDIR([src])
AC_CONFIG_HEADERS([config.h])

# Check the debug-release
AC_ARG_ENABLE(debug,
            AS_HELP_STRING([--enable-debug], [enable debugging, default: no]),
            [case "${enableval}" in
                yes) debug=true ;;
                no)  debug=false ;;
                *)   AC_MSG_ERROR([bad value ${enableval} for --enable-debug]) ;; 
                esac],
            [debug=false])

AM_CONDITIONAL(DEBUG, test x"$debug" = x"true")

if [ "$debug" = "true" ]
    then
        echo "$prefix"
        echo "$debug"
        # AC_PREFIX_DEFAULT("$(pwd)")
fi


# Checks for programs.
AC_PROG_CXX
AC_PROG_CC

# Checks for libraries.
OTHER_LIBS= opengl32 glu32 jpeg 

SFML_LIBS=sfml-graphics-s sfml-window-s sfml-system-s sfml-audio-s

# Check OS

if [ "$(uname)" == "Darwin" ]; then
    # Do something under Mac OS X platform        
elif [ "$(expr substr $(uname -s) 1 5)" == "Linux" ]; then
    # Do something under GNU/Linux platform
elif [ "$(expr substr $(uname -s) 1 10)" == "MINGW32_NT" ]; then
    # Do something under Windows NT platform
    
    OTHER_LIBS+= winmm 
fi

# Checks for header files.
# - AC_CHECK_HEADERS([headList])

# Checks for typedefs, structures, and compiler characteristics.

AC_CHECK_HEADER_STDBOOL
AC_C_INLINE
AC_TYPE_SIZE_T
AC_TYPE_UINT32_T

# Checks for library functions.

AC_OUTPUT
