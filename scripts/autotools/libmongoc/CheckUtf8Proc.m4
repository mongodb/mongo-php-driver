PHP_ARG_WITH([mongodb-utf8proc],
             [whether to use bundled or system utf8proc for SCRAM-SHA-256 SASLprep],
             [AS_HELP_STRING([--with-mongodb-utf8proc=@<:@bundled/system@:>@],
                             [MongoDB: Use bundled or system utf8proc for SCRAM-SHA-256 SASLprep [default=bundled]])],
             [bundled],
             [no])
PHP_MONGODB_VALIDATE_ARG([PHP_MONGODB_UTF8PROC], [bundled system])

found_utf8proc="no"
bundled_utf8proc="no"

AS_IF([test "$PHP_MONGODB_UTF8PROC" = "system"],[
  PKG_CHECK_MODULES([PHP_MONGODB_UTF8PROC],[libutf8proc],[
    PHP_MONGODB_BUNDLED_CFLAGS="$PHP_MONGODB_BUNDLED_CFLAGS $PHP_MONGODB_UTF8PROC_CFLAGS"
    PHP_EVAL_LIBLINE([$PHP_MONGODB_UTF8PROC_LIBS],[MONGODB_SHARED_LIBADD])
    found_utf8proc="yes"
  ],[
    PHP_CHECK_LIBRARY([utf8proc],
                      [utf8proc_NFKC],
                      [have_utf8proc_lib="yes"],
                      [have_utf8proc_lib="no"])

    AC_CHECK_HEADER([utf8proc.h],
                    [have_utf8proc_headers=yes],
                    [have_utf8proc_headers=no])

    if test "$have_utf8proc_lib" = "yes" -a "$have_utf8proc_headers" = "yes"; then
      PHP_ADD_LIBRARY([utf8proc],,[MONGODB_SHARED_LIBADD])
      found_utf8proc="yes"
    fi
  ])

  if test "$PHP_MONGODB_UTF8PROC" = "system" -a "$found_utf8proc" = "no"; then
    AC_MSG_ERROR([utf8proc library and development headers could not be found])
  fi
])

AS_IF([test "$PHP_MONGODB_UTF8PROC" = "bundled"],[
  PHP_MONGODB_UTF8PROC_CFLAGS="$PHP_MONGODB_UTF8PROC_CFLAGS -DUTF8PROC_STATIC"
  bundled_utf8proc="yes"
])
