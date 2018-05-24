AC_HEADER_STDBOOL
AC_SUBST(BSON_HAVE_STDBOOL_H, 0)
if test "$ac_cv_header_stdbool_h" = "yes"; then
    AC_SUBST(BSON_HAVE_STDBOOL_H, 1)
fi

AC_CHECK_HEADER([strings.h],
                [AC_SUBST(BSON_HAVE_STRINGS_H, 1)],
                [AC_SUBST(BSON_HAVE_STRINGS_H, 0)])
