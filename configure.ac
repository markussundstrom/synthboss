#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

m4_include([m4/AX_HAVE_QT.m4])

AC_PREREQ([2.71])
AC_INIT([Synthboss], [0.1], [BUG-REPORT-ADDRESS])
AC_CONFIG_SRCDIR([src/main.cpp])
AC_CONFIG_AUX_DIR([build_aux])
AC_CONFIG_HEADERS([config.h])

# Checks for programs.
AC_PROG_CXX

# Checks for libraries.
AX_HAVE_QT([5])

AC_PATH_PROG([MOC], [moc])
AC_SUBST([MOC])
# Checks for header files.
PKG_CHECK_MODULES([JSON], [nlohmann_json])
# Checks for typedefs, structures, and compiler characteristics.
AC_TYPE_UINT8_T

# Checks for library functions.

AC_CONFIG_FILES([Makefile
                 src/Makefile])
AM_INIT_AUTOMAKE([-Wall -Werror foreign subdir-objects -Wno-portability])
AM_SILENT_RULES([yes])
AC_OUTPUT
