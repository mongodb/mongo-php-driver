PHP_ARG_WITH([mongodb-snappy],
             [whether to enable Snappy for compression],
             [AS_HELP_STRING([--with-mongodb-snappy=@<:@auto/yes/no@:>@],
                             [MongoDB: Enable Snappy for compression [default=auto]])],
             [auto],
             [no])

PHP_ARG_WITH([mongodb-zlib],
             [whether to enable zlib for compression],
             [AS_HELP_STRING([--with-mongodb-zlib=@<:@auto/yes/system/bundled/no@:>@],
                             [MongoDB: Enable zlib for compression [default=auto]])],
             [auto],
             [no])

PHP_ARG_WITH([mongodb-zstd],
             [whether to enable zstd for compression],
             [AS_HELP_STRING([--with-mongodb-zstd=@<:@auto/yes/no@:>@],
                             [MongoDB: Enable zstd for compression [default=auto]])],
             [auto],
             [no])

found_snappy="no"
found_zlib="no"
bundled_zlib="no"
found_zstd="no"

AS_IF([test "$PHP_MONGODB_SNAPPY" = "auto" -o "$PHP_MONGODB_SNAPPY" = "yes"],[
  PKG_CHECK_MODULES([PHP_MONGODB_SNAPPY],[snappy],[
    PHP_MONGODB_BUNDLED_CFLAGS="$PHP_MONGODB_BUNDLED_CFLAGS $PHP_MONGODB_SNAPPY_CFLAGS"
    PHP_EVAL_LIBLINE([$PHP_MONGODB_SNAPPY_LIBS],[MONGODB_SHARED_LIBADD])
    found_snappy="yes"
  ],[
    PHP_CHECK_LIBRARY([snappy],
                      [snappy_uncompress],
                      [have_snappy_lib="yes"],
                      [have_snappy_lib="no"])

    AC_CHECK_HEADER([snappy-c.h],
                    [have_snappy_headers=yes],
                    [have_snappy_headers=no])

    if test "$have_snappy_lib" = "yes" -a "$have_snappy_headers" = "yes"; then
      PHP_ADD_LIBRARY([snappy],,[MONGODB_SHARED_LIBADD])
      found_snappy="yes"
    fi
  ])

  if test "$PHP_MONGODB_SNAPPY" = "yes" -a "$found_snappy" = "no"; then
    AC_MSG_ERROR([Snappy libraries and development headers could not be found])
  fi
])

AS_IF([test "$PHP_MONGODB_ZLIB" = "auto" -o "$PHP_MONGODB_ZLIB" = "yes" -o "$PHP_MONGODB_ZLIB" = "system"],[
  PKG_CHECK_MODULES([PHP_MONGODB_ZLIB],[zlib],[
    PHP_MONGODB_BUNDLED_CFLAGS="$PHP_MONGODB_BUNDLED_CFLAGS $PHP_MONGODB_ZLIB_CFLAGS"
    PHP_EVAL_LIBLINE([$PHP_MONGODB_ZLIB_LIBS],[MONGODB_SHARED_LIBADD])
    found_zlib="yes"
  ],[
    PHP_CHECK_LIBRARY([zlib],
                      [compress2],
                      [have_zlib_lib="yes"],
                      [have_zlib_lib="no"])

    AC_CHECK_HEADER([zlib.h],
                    [have_zlib_headers=yes],
                    [have_zlib_headers=no])

    if test "$have_zlib_lib" = "yes" -a "$have_zlib_headers" = "yes"; then
      PHP_ADD_LIBRARY([z],,[MONGODB_SHARED_LIBADD])
      found_zlib="yes"
    fi
  ])

  if test "$PHP_MONGODB_ZLIB" = "system" -a "$found_zlib" = "no"; then
    AC_MSG_ERROR([zlib libraries and development headers could not be found])
  fi
])

dnl Use libmongoc's bundled zlib if necessary
AS_IF([test "$found_zlib" = "no" -a \( "$PHP_MONGODB_ZLIB" = "auto" -o "$PHP_MONGODB_ZLIB" = "yes" -o "$PHP_MONGODB_ZLIB" = "bundled" \)],[
  AC_CHECK_HEADER([unistd.h],[PHP_MONGODB_ZLIB_CFLAGS="$PHP_MONGODB_ZLIB_CFLAGS -DHAVE_UNISTD_H"])
  AC_CHECK_HEADER([stdarg.h],[PHP_MONGODB_ZLIB_CFLAGS="$PHP_MONGODB_ZLIB_CFLAGS -DHAVE_STDARG_H"])
  bundled_zlib="yes"
])

AS_IF([test "$PHP_MONGODB_ZSTD" = "auto" -o "$PHP_MONGODB_ZSTD" = "yes"],[
  PKG_CHECK_MODULES([PHP_MONGODB_ZSTD],[libzstd],[
    PHP_MONGODB_BUNDLED_CFLAGS="$PHP_MONGODB_BUNDLED_CFLAGS $PHP_MONGODB_ZSTD_CFLAGS"
    PHP_EVAL_LIBLINE([$PHP_MONGODB_ZSTD_LIBS],[MONGODB_SHARED_LIBADD])
    found_zstd="yes"
  ],[
    PHP_CHECK_LIBRARY([zstd],
                      [ZSTD_compress],
                      [have_zstd_lib="yes"],
                      [have_zstd_lib="no"])

    AC_CHECK_HEADER([zstd.h],
                    [have_zstd_headers=yes],
                    [have_zstd_headers=no])

    if test "$have_zstd_lib" = "yes" -a "$have_zstd_headers" = "yes"; then
      PHP_ADD_LIBRARY([zstd],,[MONGODB_SHARED_LIBADD])
      found_zstd="yes"
    fi
  ])

  if test "$PHP_MONGODB_ZSTD" = "yes" -a "$found_zstd" = "no"; then
    AC_MSG_ERROR([zstd libraries and development headers could not be found])
  fi
])

if test "$found_snappy" = "yes" -o "$found_zlib" = "yes" -o "$bundled_zlib" = "yes" -o "$found_zstd" = "yes"; then
  AC_SUBST(MONGOC_ENABLE_COMPRESSION, 1)
  if test "$found_snappy" = "yes"; then
    AC_SUBST(MONGOC_ENABLE_COMPRESSION_SNAPPY, 1)
  else
    AC_SUBST(MONGOC_ENABLE_COMPRESSION_SNAPPY, 0)
  fi
  if test "$found_zlib" = "yes" -o "$bundled_zlib" = "yes"; then
    AC_SUBST(MONGOC_ENABLE_COMPRESSION_ZLIB, 1)
  else
    AC_SUBST(MONGOC_ENABLE_COMPRESSION_ZLIB, 0)
  fi
  if test "$found_zstd" = "yes"; then
    AC_SUBST(MONGOC_ENABLE_COMPRESSION_ZSTD, 1)
  else
    AC_SUBST(MONGOC_ENABLE_COMPRESSION_ZSTD, 0)
  fi
else
  AC_SUBST(MONGOC_ENABLE_COMPRESSION, 0)
  AC_SUBST(MONGOC_ENABLE_COMPRESSION_SNAPPY, 0)
  AC_SUBST(MONGOC_ENABLE_COMPRESSION_ZLIB, 0)
  AC_SUBST(MONGOC_ENABLE_COMPRESSION_ZSTD, 0)
fi
