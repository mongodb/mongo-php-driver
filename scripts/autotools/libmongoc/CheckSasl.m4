PHP_ARG_WITH([mongodb-sasl],
             [whether to enable SASL for Kerberos authentication],
             [AS_HELP_STRING([--with-mongodb-sasl=@<:@auto/cyrus/no@:>@],
                             [MongoDB: Enable SASL for Kerberos authentication [default=auto]])],
             [auto],
             [no])

dnl PHP_ARG_WITH without a value assigns "yes". Treat it like "cyrus" (required)
if test "$PHP_MONGODB_SASL" = "yes"; then
  PHP_MONGODB_SASL="cyrus"
fi

PHP_MONGODB_VALIDATE_ARG([PHP_MONGODB_SASL], [auto cyrus no])

AS_IF([test "$PHP_MONGODB_SASL" = "auto" -o "$PHP_MONGODB_SASL" = "cyrus"],[
  found_cyrus="no"

  PKG_CHECK_MODULES([PHP_MONGODB_SASL],[libsasl2],[
    PHP_MONGODB_BUNDLED_CFLAGS="$PHP_MONGODB_BUNDLED_CFLAGS $PHP_MONGODB_SASL_CFLAGS"
    PHP_EVAL_LIBLINE([$PHP_MONGODB_SASL_LIBS],[MONGODB_SHARED_LIBADD])
    PHP_MONGODB_SASL="cyrus"
    found_cyrus="yes"
  ],[
    PHP_CHECK_LIBRARY([sasl2],
                      [sasl_client_init],
                      [have_sasl2_lib="yes"],
                      [have_sasl2_lib="no"])

    AC_CHECK_HEADER([sasl/sasl.h],
                    [have_sasl_headers=yes],
                    [have_sasl_headers=no])

    if test "$have_sasl2_lib" = "yes" -a "$have_sasl_headers" = "yes"; then
      PHP_ADD_LIBRARY([sasl2],,[MONGODB_SHARED_LIBADD])
      PHP_MONGODB_SASL="cyrus"
      found_cyrus="yes"
    fi
  ])

  if test "$found_cyrus" = "yes"; then
    PHP_CHECK_LIBRARY([sasl2],
                      [sasl_client_done],
                      [have_sasl_client_done=yes],
                      [have_sasl_client_done=no],
                      $MONGODB_SHARED_LIBADD)
  fi

  if test "$PHP_MONGODB_SASL" = "cyrus" -a "$found_cyrus" != "yes"; then
    AC_MSG_ERROR([Cyrus SASL libraries and development headers could not be found])
  fi
])

if test "$PHP_MONGODB_SASL" = "auto"; then
  PHP_MONGODB_SASL="no"
fi

AC_MSG_CHECKING([which SASL library to use])
AC_MSG_RESULT([$PHP_MONGODB_SASL])

dnl Disable Windows SSPI
AC_SUBST(MONGOC_ENABLE_SASL_SSPI, 0)

if test "$PHP_MONGODB_SASL" = "cyrus"; then
  AC_SUBST(MONGOC_ENABLE_SASL, 1)
  AC_SUBST(MONGOC_ENABLE_SASL_CYRUS, 1)
  if test "$have_sasl_client_done" = "yes"; then
    AC_SUBST(MONGOC_HAVE_SASL_CLIENT_DONE, 1)
  else
    AC_SUBST(MONGOC_HAVE_SASL_CLIENT_DONE, 0)
  fi
else
  AC_SUBST(MONGOC_ENABLE_SASL, 0)
  AC_SUBST(MONGOC_ENABLE_SASL_CYRUS, 0)
  AC_SUBST(MONGOC_HAVE_SASL_CLIENT_DONE, 0)
fi
