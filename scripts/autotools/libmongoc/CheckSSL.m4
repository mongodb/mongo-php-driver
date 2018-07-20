PHP_ARG_WITH([mongodb-ssl],
             [whether to enable crypto and TLS],
             [AS_HELP_STRING([--with-mongodb-ssl=@<:@auto/no/openssl/libressl/darwin@:>@],
                             [MongoDB: Enable TLS connections and SCRAM-SHA-1 authentication [default=auto]])],
             [auto],
             [no])

PHP_ARG_WITH([openssl-dir],
             [deprecated option for OpenSSL library path],
             [AS_HELP_STRING([--with-openssl-dir=@<:@auto/DIR@:>@],
                             [MongoDB: OpenSSL library path (deprecated for pkg-config) [default=auto]])],
             [auto],
             [no])

dnl PHP_ARG_WITH without a value assigns "yes". Treat it like "auto" but required.
AS_IF([test "$PHP_MONGODB_SSL" = "yes"],[
  crypto_required="yes"
  PHP_MONGODB_SSL="auto"
])

AS_IF([test "$PHP_MONGODB_SSL" = "darwin" -o \( "$PHP_MONGODB_SSL" = "auto" -a "$os_darwin" = "yes" \)],[
  AC_MSG_NOTICE([checking whether Darwin SSL is available])

  if test "$os_darwin" = "no"; then
    AC_MSG_ERROR([Darwin SSL is only supported on macOS])
  fi
  dnl PHP_FRAMEWORKS is only used for SAPI builds, so use MONGODB_SHARED_LIBADD for shared builds
  if test "$ext_shared" = "yes"; then
    MONGODB_SHARED_LIBADD="-framework Security -framework CoreFoundation $MONGODB_SHARED_LIBADD"
  else
    PHP_ADD_FRAMEWORK([Security])
    PHP_ADD_FRAMEWORK([CoreFoundation])
  fi
  PHP_MONGODB_SSL="darwin"
])

AS_IF([test "$PHP_MONGODB_SSL" = "openssl" -o "$PHP_MONGODB_SSL" = "auto"],[
  AC_MSG_NOTICE([checking whether OpenSSL is available])
  found_openssl="no"

  PKG_CHECK_MODULES([PHP_MONGODB_SSL],[openssl],[
    PHP_MONGODB_BUNDLED_CFLAGS="$PHP_MONGODB_BUNDLED_CFLAGS $PHP_MONGODB_SSL_CFLAGS"
    PHP_EVAL_LIBLINE([$PHP_MONGODB_SSL_LIBS],[MONGODB_SHARED_LIBADD])
    PHP_MONGODB_SSL="openssl"
    found_openssl="yes"

    old_CFLAGS="$CFLAGS"
    CFLAGS="$PHP_MONGODB_SSL_CFLAGS $CFLAGS"

    AC_CHECK_DECLS([ASN1_STRING_get0_data],
                   [have_ASN1_STRING_get0_data="yes"],
                   [have_ASN1_STRING_get0_data="no"],
                   [[#include <openssl/asn1.h>]])

    CFLAGS="$old_CFLAGS"
  ],[
    unset OPENSSL_INCDIR
    unset OPENSSL_LIBDIR

    dnl Use a list of directories from PHP_SETUP_OPENSSL by default.
    dnl Support documented "auto" and older, undocumented "yes" options
    if test "$PHP_OPENSSL_DIR" = "auto" -o "$PHP_OPENSSL_DIR" = "yes"; then
      PHP_OPENSSL_DIR="/usr/local/ssl /usr/local /usr /usr/local/openssl"
    fi

    for i in $PHP_OPENSSL_DIR; do
      if test -r $i/include/openssl/evp.h; then
        OPENSSL_INCDIR="$i/include"
      fi
      if test -r $i/$PHP_LIBDIR/libssl.a -o -r $i/$PHP_LIBDIR/libssl.$SHLIB_SUFFIX_NAME; then
        OPENSSL_LIBDIR="$i/$PHP_LIBDIR"
      fi
      test -n "$OPENSSL_INCDIR" && test -n "$OPENSSL_LIBDIR" && break
    done

    if test -n "$OPENSSL_LIBDIR"; then
      OPENSSL_LIBDIR_LDFLAG="-L$OPENSSL_LIBDIR"
    fi

    PHP_CHECK_LIBRARY([crypto],
                      [EVP_DigestInit_ex],
                      [have_crypto_lib="yes"],
                      [have_crypto_lib="no"],
                      [$OPENSSL_LIBDIR_LDFLAG])


    AC_MSG_NOTICE([checking whether OpenSSL >= 1.1.0 is available])
    PHP_CHECK_LIBRARY([ssl],
                      [OPENSSL_init_ssl],
                      [have_ssl_lib="yes"],
                      [have_ssl_lib="no"],
                      [$OPENSSL_LIBDIR_LDFLAG -lcrypto])

    if test "$have_ssl_lib" = "no"; then
        AC_MSG_NOTICE([checking whether OpenSSL < 1.1.0 is available])
        PHP_CHECK_LIBRARY([ssl],
                          [SSL_library_init],
                          [have_ssl_lib="yes"],
                          [have_ssl_lib="no"],
                          [$OPENSSL_LIBDIR_LDFLAG -lcrypto])
    fi

    if test "$have_ssl_lib" = "yes" -a "$have_crypto_lib" = "yes"; then
      PHP_ADD_LIBRARY([ssl],,[MONGODB_SHARED_LIBADD])
      PHP_ADD_LIBRARY([crypto],,[MONGODB_SHARED_LIBADD])

      if test -n "$OPENSSL_LIBDIR"; then
        PHP_ADD_LIBPATH([$OPENSSL_LIBDIR],[MONGODB_SHARED_LIBADD])
      fi

      if test -n "$OPENSSL_INCDIR"; then
        PHP_ADD_INCLUDE($OPENSSL_INCDIR)
      fi

      old_CFLAGS="$CFLAGS"
      CFLAGS="-I$OPENSSL_INCDIR $CFLAGS"

      AC_CHECK_DECLS([ASN1_STRING_get0_data],
                     [have_ASN1_STRING_get0_data="yes"],
                     [have_ASN1_STRING_get0_data="no"],
                     [[#include <openssl/asn1.h>]])

      CFLAGS="$old_CFLAGS"

      PHP_MONGODB_SSL="openssl"
      found_openssl="yes"
    fi
  ])

  if test "$PHP_MONGODB_SSL" = "openssl" -a "$found_openssl" != "yes"; then
    AC_MSG_ERROR([OpenSSL libraries and development headers could not be found])
  fi
])

AS_IF([test "$PHP_MONGODB_SSL" = "libressl" -o "$PHP_MONGODB_SSL" = "auto"],[
  AC_MSG_NOTICE([checking whether LibreSSL is available])
  found_libressl="no"

  PKG_CHECK_MODULES([PHP_MONGODB_SSL],[libtls libcrypto],[
    PHP_MONGODB_BUNDLED_CFLAGS="$PHP_MONGODB_BUNDLED_CFLAGS $PHP_MONGODB_SSL_CFLAGS"
    PHP_EVAL_LIBLINE([$PHP_MONGODB_SSL_LIBS],[MONGODB_SHARED_LIBADD])
    PHP_MONGODB_SSL="libressl"
    found_libressl="yes"
  ],[
    PHP_CHECK_LIBRARY([crypto],
                      [EVP_DigestInit_ex],
                      [have_crypto_lib="yes"],
                      [have_crypto_lib="no"])
    PHP_CHECK_LIBRARY([tls],
                      [tls_init],
                      [have_ssl_lib="yes"],
                      [have_ssl_lib="no"],
                      [-lcrypto])

    if test "$have_ssl_lib" = "yes" -a "$have_crypto_lib" = "yes"; then
      PHP_ADD_LIBRARY([tls],,[MONGODB_SHARED_LIBADD])
      PHP_ADD_LIBRARY([crypto],,[MONGODB_SHARED_LIBADD])
      PHP_MONGODB_SSL="libressl"
      found_libressl="yes"
    fi
  ])

  if test "$PHP_MONGODB_SSL" = "libressl" -a "$found_libressl" != "yes"; then
    AC_MSG_ERROR([LibreSSL libraries and development headers could not be found])
  fi
])

AS_IF([test "$PHP_MONGODB_SSL" = "auto"],[
  if test "x$crypto_required" = "xyes"; then
    AC_MSG_ERROR([crypto and TLS libraries not found])
  fi
  PHP_MONGODB_SSL="no"
])

AC_MSG_CHECKING([which TLS library to use])
AC_MSG_RESULT([$PHP_MONGODB_SSL])

dnl Disable Windows SSL and crypto
AC_SUBST(MONGOC_ENABLE_SSL_SECURE_CHANNEL, 0)
AC_SUBST(MONGOC_ENABLE_CRYPTO_CNG, 0)

if test "$PHP_MONGODB_SSL" = "openssl" -o "$PHP_MONGODB_SSL" = "libressl" -o "$PHP_MONGODB_SSL" = "darwin"; then
  AC_SUBST(MONGOC_ENABLE_SSL, 1)
  AC_SUBST(MONGOC_ENABLE_CRYPTO, 1)
  if test "$PHP_MONGODB_SSL" = "darwin"; then
    AC_SUBST(MONGOC_ENABLE_SSL_OPENSSL, 0)
    AC_SUBST(MONGOC_ENABLE_SSL_LIBRESSL, 0)
    AC_SUBST(MONGOC_ENABLE_SSL_SECURE_TRANSPORT, 1)
    AC_SUBST(MONGOC_ENABLE_CRYPTO_LIBCRYPTO, 0)
    AC_SUBST(MONGOC_ENABLE_CRYPTO_COMMON_CRYPTO, 1)
  elif test "$PHP_MONGODB_SSL" = "openssl"; then
    AC_SUBST(MONGOC_ENABLE_SSL_OPENSSL, 1)
    AC_SUBST(MONGOC_ENABLE_SSL_LIBRESSL, 0)
    AC_SUBST(MONGOC_ENABLE_SSL_SECURE_TRANSPORT, 0)
    AC_SUBST(MONGOC_ENABLE_CRYPTO_LIBCRYPTO, 1)
    AC_SUBST(MONGOC_ENABLE_CRYPTO_COMMON_CRYPTO, 0)
  elif test "$PHP_MONGODB_SSL" = "libressl"; then
    AC_SUBST(MONGOC_ENABLE_SSL_OPENSSL, 0)
    AC_SUBST(MONGOC_ENABLE_SSL_LIBRESSL, 1)
    AC_SUBST(MONGOC_ENABLE_SSL_SECURE_TRANSPORT, 0)
    AC_SUBST(MONGOC_ENABLE_CRYPTO_LIBCRYPTO, 1)
    AC_SUBST(MONGOC_ENABLE_CRYPTO_COMMON_CRYPTO, 0)
  fi
else
  AC_SUBST(MONGOC_ENABLE_SSL, 0)
  AC_SUBST(MONGOC_ENABLE_SSL_LIBRESSL, 0)
  AC_SUBST(MONGOC_ENABLE_SSL_OPENSSL, 0)
  AC_SUBST(MONGOC_ENABLE_SSL_SECURE_TRANSPORT, 0)
  AC_SUBST(MONGOC_ENABLE_CRYPTO, 0)
  AC_SUBST(MONGOC_ENABLE_CRYPTO_LIBCRYPTO, 0)
  AC_SUBST(MONGOC_ENABLE_CRYPTO_COMMON_CRYPTO, 0)
fi

if test "x$have_ASN1_STRING_get0_data" = "xyes"; then
  AC_SUBST(MONGOC_HAVE_ASN1_STRING_GET0_DATA, 1)
else
  AC_SUBST(MONGOC_HAVE_ASN1_STRING_GET0_DATA, 0)
fi

PHP_ARG_ENABLE([mongodb-crypto-system-profile],
               [whether to use system crypto profile],
               [AS_HELP_STRING([--enable-mongodb-crypto-system-profile],
                               [MongoDB: Use system crypto profile (OpenSSL only) [default=no]])],
               [no],
               [no])

PHP_ARG_WITH([system-ciphers],
             [deprecated option for whether to use system crypto profile],
             AS_HELP_STRING([--enable-system-ciphers],
                            [MongoDB: whether to use system crypto profile (deprecated for --enable-mongodb-crypto-system-profile) [default=no]]),
             [no],
             [no])

dnl Also consider the deprecated --enable-system-ciphers option
if test "$PHP_MONGODB_CRYPTO_SYSTEM_PROFILE" = "yes" -o "$PHP_SYSTEM_CIPHERS" = "yes"; then
  if test "$PHP_MONGODB_SSL" = "openssl"; then
    AC_SUBST(MONGOC_ENABLE_CRYPTO_SYSTEM_PROFILE, 1)
  else
    AC_MSG_ERROR([System crypto profile is only available with OpenSSL])
  fi
else
  AC_SUBST(MONGOC_ENABLE_CRYPTO_SYSTEM_PROFILE, 0)
fi
