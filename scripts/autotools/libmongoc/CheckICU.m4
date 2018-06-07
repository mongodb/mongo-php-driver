PHP_ARG_WITH([mongodb-icu],
             [whether to enable ICU for SASLPrep with SCRAM-SHA-256 authentication],
             [AS_HELP_STRING([--with-mongodb-icu=@<:@auto/yes/no@:>@],
                             [MongoDB: Enable ICU for SASLPrep with SCRAM-SHA-256 authentication [default=auto]])],
             [auto],
             [no])

AS_IF([test "$PHP_MONGODB_ICU" = "auto" -o "$PHP_MONGODB_ICU" = "yes"],[
  found_icu=no

  PKG_CHECK_MODULES([PHP_MONGODB_ICU],[icu-uc],[
    PHP_MONGODB_BUNDLED_CFLAGS="$PHP_MONGODB_BUNDLED_CFLAGS $PHP_MONGODB_ICU_CFLAGS"
    PHP_EVAL_LIBLINE([$PHP_MONGODB_ICU_LIBS],[MONGODB_SHARED_LIBADD])
    PHP_MONGODB_ICU="yes"
    found_icu=yes
  ],[
    found_icu=no
  ])

  if test "$PHP_MONGODB_ICU" = "yes" -a "$found_icu" = "no"; then
    AC_MSG_ERROR([ICU libraries and development headers could not be found])
  fi
])

AS_IF([test "$PHP_MONGODB_ICU" = "auto"],[
  PHP_MONGODB_ICU="no"
])

if test ! \( "$PHP_MONGODB_ICU" = "yes" -o "$PHP_MONGODB_ICU" = "no" \); then
  AC_MSG_WARN([unsupported --with-mongodb-icu value: $PHP_MONGODB_ICU])
fi

if test "$PHP_MONGODB_ICU" = "yes"; then
  AC_SUBST(MONGOC_ENABLE_ICU, 1)
else
  AC_SUBST(MONGOC_ENABLE_ICU, 0)
fi
