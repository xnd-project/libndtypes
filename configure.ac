
AC_PREREQ([2.67])
AC_INIT(ndtypes, 0.1.0, plures@anaconda.com, ndtypes, https://github.com/plures/)
AC_CONFIG_HEADER(config.h)
AC_CONFIG_FILES([Makefile libndtypes/Makefile libndtypes/tests/Makefile])

LIBSTATIC=libndtypes.a
LIBSONAME=libndtypes.so.0
LIBSHARED=libndtypes.so.0.1.0
AC_SUBST(LIBSTATIC)
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


# Compiler dependent settings:
NDT_WARN=
NDT_OPT="-O2"
NDT_PGEN=
NDT_PUSE=
case $CC in
    *gcc*)
        NDT_WARN="-Wall -Wextra -std=c11 -pedantic"
        NDT_OPT="-O2"
        NDT_PGEN="-fprofile-generate -fprofile-values"
        NDT_PUSE="-fprofile-use -freorder-blocks"
        ;;
    *icc*)
        AR=xiar
        NDT_WARN="-Wall"
        NDT_OPT="-O2"
        NDT_PGEN="-wd11505 -prof-gen"
        NDT_PUSE="-wd11505 -prof-use"
        ;;
    *clang*)
        NDT_WARN="-Wall -Wextra -std=c11 -pedantic"
        NDT_OPT="-O2"
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

AC_SUBST(NDT_PGEN)
AC_SUBST(NDT_PUSE)

if test -z "$CFLAGS"; then
    CONFIGURE_CFLAGS="$NDT_WARN $NDT_CONFIG $NDT_OPT"
else
    CONFIGURE_CFLAGS="$NDT_WARN $NDT_CONFIG $NDT_OPT $CFLAGS"
fi

if test -n "$LDFLAGS"; then
    CONFIGURE_LDFLAGS="$LDFLAGS"
fi

AC_SUBST(CONFIGURE_CFLAGS)
AC_SUBST(CONFIGURE_LDFLAGS)

AC_OUTPUT

