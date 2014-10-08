dnl Process this file with autoconf to produce a configure script.

AC_PREREQ(2.26)

m4_define([baculua_major_version], [0])
m4_define([baculua_minor_version], [1])
m4_define([baculua_version], [baculua_major_version.baculua_minor_version])

AC_INIT([baculua],[1.0])
AC_CONFIG_MACRO_DIR([config])
AM_INIT_AUTOMAKE([1.11 dist-bzip2])
LT_PREREQ([2.2])
LT_INIT([dlopen])

AC_SUBST(BACULUA_MAJOR_VERSION, [baculua_major_version])
AC_SUBST(BACULUA_MINOR_VERSION, [baculua_minor_version])
AC_SUBST(BACULUA_VERSION, [baculua_version])

dnl Check for programs

AC_CONFIG_FILES([Makefile])
AC_CONFIG_FILES([src/Makefile])
AC_CONFIG_FILES([tests/Makefile])

# tests/monitor
AC_CONFIG_FILES([tests/monitor/Makefile])
AC_CONFIG_FILES([tests/monitor/src/Makefile])
# tests/util
AC_CONFIG_FILES([tests/util/Makefile])
AC_CONFIG_FILES([tests/util/src/Makefile])
# tests/baculua
AC_CONFIG_FILES([tests/baculua/Makefile])
AC_CONFIG_FILES([tests/baculua/src/Makefile])
AC_OUTPUT