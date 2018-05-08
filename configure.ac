
AC_PREREQ([2.67])
AC_INIT(ndtypes, 0.2.0dev3, skrah@bytereef.org, ndtypes, https://github.com/plures/)
AC_CONFIG_HEADER(config.h)
AC_CONFIG_FILES([
  Makefile
  libndtypes/Makefile libndtypes/tests/Makefile libndtypes/ndtypes.h
  libndtypes/compat/Makefile
  libndtypes/serialize/Makefile
])

# System and machine type (only used for Darwin):
AC_MSG_CHECKING(system as reported by uname -s)
ac_sys_system=`uname -s`
AC_MSG_RESULT($ac_sys_system)

LIBSTATIC=libndtypes.a
case $ac_sys_system in
    darwin*|Darwin*)
        LIBRARY_PATH="DYLD_LIBRARY_PATH"
        LIBNAME="libndtypes.dylib"
        LIBSONAME="libndtypes.0.dylib"
        LIBSHARED="libndtypes.0.2.0dev3.dylib"
        CONFIGURE_LDFLAGS="-dynamiclib -install_name @rpath/$LIBSONAME -compatibility_version 0.2 -current_version 0.2.0"
        ;;
    *)
        LIBRARY_PATH="LD_LIBRARY_PATH"
        LIBNAME="libndtypes.so"
        LIBSONAME="libndtypes.so.0"
        LIBSHARED="libndtypes.so.0.2.0dev3"
        CONFIGURE_LDFLAGS="-shared -Wl,-soname,$LIBSONAME"
        ;;
esac

AC_SUBST(LIBSTATIC)
AC_SUBST(LIBRARY_PATH)
AC_SUBST(LIBNAME)
AC_SUBST(LIBSONAME)
AC_SUBST(LIBSHARED)


# Apparently purely informational for this particular build:
AC_CANONICAL_HOST
AC_SUBST(build)
AC_SUBST(host)

# Language and compiler:
AC_LANG_C
saved_cflags=$CFLAGS
AC_PROG_CC
CFLAGS=$saved_cflags

# ar and ranlib:
AC_CHECK_TOOL(AR, ar, ar)
AC_PROG_RANLIB
AC_SUBST(RANLIB)


# Checks for header files:
AC_HEADER_STDC

# Install program:
AC_PROG_INSTALL
AC_SUBST(INSTALL)

# Endianness:
AC_C_BIGENDIAN

if test "$ac_cv_c_bigendian" = "yes"; then
  NDT_SYS_BIG_ENDIAN=1
else
  NDT_SYS_BIG_ENDIAN=0
fi

AC_SUBST(NDT_SYS_BIG_ENDIAN)

# Exact memcheck:
AC_MSG_CHECKING([for --with-valgrind])
AC_ARG_WITH([valgrind],
  AS_HELP_STRING([--with-valgrind], [enable exact memcheck]),,
  with_valgrind=no)
AC_MSG_RESULT([$with_valgrind])
if test "$with_valgrind" != no; then
    AC_CHECK_HEADER([valgrind/memcheck.h],
      [AC_DEFINE([WITH_VALGRIND], 1, [required for exact memcheck])],
      [AC_MSG_ERROR([exact memcheck requested but headers not available])]
    )
fi

# Without documentation:
AC_MSG_CHECKING([for --with-docs])
AC_ARG_WITH([docs],
  AS_HELP_STRING([--with-docs], [install documentation - enabled by default]),,
  with_docs=yes)
AC_MSG_RESULT([$with_docs])
if test "$with_docs" = yes; then
   NDT_INSTALL_DOCS="install_docs"
else
   NDT_INSTALL_DOCS=""
fi

AC_SUBST(NDT_INSTALL_DOCS)

# Compiler dependent settings:
NDT_WARN=
NDT_OPT="-O2 -g"
case $CC in
    *gcc*)
        NDT_WARN="-Wall -Wextra -std=c11 -pedantic"
        NDT_OPT="-O2 -g"
        NDT_COV_CFLAGS="-O0 -g -fno-inline -fprofile-arcs -ftest-coverage"
        NDT_COV_LDFLAGS="-fprofile-arcs"
        ;;
    *icc*)
        AR=xiar
        NDT_WARN="-Wall"
        NDT_OPT="-O2 -g"
        NDT_COV_CFLAGS=""
        NDT_COV_LDFLAGS=""
        ;;
    *clang*)
        NDT_WARN="-Wall -Wextra -std=c11 -pedantic"
        NDT_OPT="-O2 -g"
        NDT_COV_CFLAGS="-O0 -g -fno-inline -fprofile-arcs -ftest-coverage"
        NDT_COV_LDFLAGS="-fprofile-arcs"
        ;;
esac


# Substitute variables and generate output:
if test -z "$LD"; then
    LD="$CC"
fi
AC_SUBST(LD)
AC_SUBST(AR)
AC_SUBST(NDT_WARN)
AC_SUBST(NDT_OPT)


if test -z "$CFLAGS"; then
    CONFIGURE_CFLAGS="-I.. $NDT_WARN $NDT_OPT"
    CONFIGURE_COV_CFLAGS="-I.. $NDT_WARN $NDT_COV_CFLAGS"
else
    CONFIGURE_CFLAGS="-I.. $NDT_WARN $NDT_OPT $CFLAGS"
    CONFIGURE_COV_CFLAGS="-I.. $NDT_WARN $NDT_COV_CFLAGS $CFLAGS"
fi

CONFIGURE_COV_LDFLAGS="$CONFIGURE_LDFLAGS $NDT_COV_LDFLAGS"
if test -n "$LDFLAGS"; then
    CONFIGURE_LDFLAGS="$CONFIGURE_LDFLAGS $LDFLAGS"
    CONFIGURE_COV_LDFLAGS="$CONFIGURE_COV_LDFLAGS $LDFLAGS"
fi

AC_SUBST(CONFIGURE_CFLAGS)
AC_SUBST(CONFIGURE_LDFLAGS)

AC_SUBST(CONFIGURE_COV_CFLAGS)
AC_SUBST(CONFIGURE_COV_LDFLAGS)

AC_OUTPUT

