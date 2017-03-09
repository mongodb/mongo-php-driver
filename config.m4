dnl config.m4 for extension mongodb
PHP_ARG_ENABLE(mongodb, whether to enable mongodb support,
[  --enable-mongodb           Enable mongodb support])
PHP_ARG_WITH(openssl-dir, OpenSSL dir for mongodb,
[  --with-openssl-dir[=DIR]  openssl install prefix], yes, no)
PHP_ARG_WITH(system-ciphers, whether to use system default cipher list instead of hardcoded value,
[  --with-system-ciphers   OPENSSL: Use system default cipher list instead of hardcoded value], no, no)

dnl borrowed from libmongoc configure.ac
# AS_VAR_COPY is available in AC 2.64 and on, but we only require 2.60.
# If we're on an older version, we define it ourselves:
m4_ifndef([AS_VAR_COPY],
          [m4_define([AS_VAR_COPY],
          [AS_LITERAL_IF([$1[]$2], [$1=$$2], [eval $1=\$$2])])])

# Get "user-set cflags" here, before we've added the flags we use by default
AS_VAR_COPY(MONGOC_USER_SET_CFLAGS, [CFLAGS])
AC_SUBST(MONGOC_USER_SET_CFLAGS)

AS_VAR_COPY(MONGOC_USER_SET_LDFLAGS, [LDFLAGS])
AC_SUBST(MONGOC_USER_SET_LDFLAGS)

AS_VAR_COPY(MONGOC_CC, [CC])
AC_SUBST(MONGOC_CC)

dnl borrowed from PHP acinclude.m4
AC_DEFUN([PHP_BSON_BIGENDIAN],
[AC_CACHE_CHECK([whether byte ordering is bigendian], ac_cv_c_bigendian_php,
 [
  ac_cv_c_bigendian_php=unknown
  AC_TRY_RUN(
  [
int main(void)
{
  short one = 1;
  char *cp = (char *)&one;

  if (*cp == 0) {
    return(0);
  } else {
    return(1);
  }
}
  ], [ac_cv_c_bigendian_php=yes], [ac_cv_c_bigendian_php=no], [ac_cv_c_bigendian_php=unknown])
 ])
 if test $ac_cv_c_bigendian_php = yes; then
    AC_SUBST(BSON_BYTE_ORDER, 4321)
  else
    AC_SUBST(BSON_BYTE_ORDER, 1234)
 fi
])
dnl Borrowed from sapi/fpm/config.m4
AC_DEFUN([PHP_BSON_CLOCK],
[
  have_clock_gettime=no

  AC_MSG_CHECKING([for clock_gettime])

  AC_TRY_LINK([ #include <time.h> ], [struct timespec ts; clock_gettime(CLOCK_MONOTONIC, &ts);], [
    have_clock_gettime=yes
    AC_MSG_RESULT([yes])
  ], [
    AC_MSG_RESULT([no])
  ])

  if test "$have_clock_gettime" = "no"; then
    AC_MSG_CHECKING([for clock_gettime in -lrt])

    SAVED_LIBS="$LIBS"
    LIBS="$LIBS -lrt"

    AC_TRY_LINK([ #include <time.h> ], [struct timespec ts; clock_gettime(CLOCK_MONOTONIC, &ts);], [
      have_clock_gettime=yes
      AC_MSG_RESULT([yes])
    ], [
      LIBS="$SAVED_LIBS"
      AC_MSG_RESULT([no])
    ])
  fi

  if test "$have_clock_gettime" = "yes"; then
    AC_SUBST(BSON_HAVE_CLOCK_GETTIME, 1)
  fi
])

AC_MSG_CHECKING(PHP version)
PHP_FOUND_VERSION=`${PHP_CONFIG} --version`
PHP_FOUND_VERNUM=`echo "${PHP_FOUND_VERSION}" | $AWK 'BEGIN { FS = "."; } { printf "%d", ([$]1 * 100 + [$]2) * 100 + [$]3;}'`
AC_MSG_RESULT($PHP_FOUND_VERNUM)

if test "$MONGODB" != "no"; then
  PHP_ARG_ENABLE(developer-flags, whether to enable developer build flags,
  [  --enable-developer-flags   Enable developer flags],, no)

  if test "$PHP_DEVELOPER_FLAGS" = "yes"; then
    dnl Warn about functions which might be candidates for format attributes
    PHP_CHECK_GCC_ARG(-Wmissing-format-attribute,       _MAINTAINER_CFLAGS="$_MAINTAINER_CFLAGS -Wmissing-format-attribute")
    dnl Avoid duplicating values for an enum
    PHP_CHECK_GCC_ARG(-Wduplicate-enum,                 _MAINTAINER_CFLAGS="$_MAINTAINER_CFLAGS -Wduplicate-enum")
    dnl Warns on mismatches between #ifndef and #define header guards
    PHP_CHECK_GCC_ARG(-Wheader-guard,                   _MAINTAINER_CFLAGS="$_MAINTAINER_CFLAGS -Wheader-guard")
    dnl logical not of a non-boolean expression
    PHP_CHECK_GCC_ARG(-Wlogical-not-parentheses,        _MAINTAINER_CFLAGS="$_MAINTAINER_CFLAGS -Wlogical-not-parentheses")
    dnl Warn about suspicious uses of logical operators in expressions
    PHP_CHECK_GCC_ARG(-Wlogical-op,                     _MAINTAINER_CFLAGS="$_MAINTAINER_CFLAGS -Wlogical-op")
    dnl memory error detector.
    dnl FIXME: -fsanitize=address,undefined for clang. The PHP_CHECK_GCC_ARG macro isn't happy about that string :(
    PHP_CHECK_GCC_ARG(-fsanitize-address,               _MAINTAINER_CFLAGS="$_MAINTAINER_CFLAGS -fsanitize-address")
    dnl Enable frame debugging
    PHP_CHECK_GCC_ARG(-fno-omit-frame-pointer,          _MAINTAINER_CFLAGS="$_MAINTAINER_CFLAGS -fno-omit-frame-pointer")
    dnl Make sure we don't optimize calls
    PHP_CHECK_GCC_ARG(-fno-optimize-sibling-calls,      _MAINTAINER_CFLAGS="$_MAINTAINER_CFLAGS -fno-optimize-sibling-calls")
    PHP_CHECK_GCC_ARG(-Wlogical-op-parentheses,         _MAINTAINER_CFLAGS="$_MAINTAINER_CFLAGS -Wlogical-op-parentheses")
    PHP_CHECK_GCC_ARG(-Wbool-conversion,                _MAINTAINER_CFLAGS="$_MAINTAINER_CFLAGS -Wbool-conversion")
    PHP_CHECK_GCC_ARG(-Wloop-analysis,                  _MAINTAINER_CFLAGS="$_MAINTAINER_CFLAGS -Wloop-analysis")
    PHP_CHECK_GCC_ARG(-Wsizeof-array-argument,          _MAINTAINER_CFLAGS="$_MAINTAINER_CFLAGS -Wsizeof-array-argument")
    PHP_CHECK_GCC_ARG(-Wstring-conversion,              _MAINTAINER_CFLAGS="$_MAINTAINER_CFLAGS -Wstring-conversion")
    PHP_CHECK_GCC_ARG(-Wno-variadic-macros,             _MAINTAINER_CFLAGS="$_MAINTAINER_CFLAGS -Wno-variadic-macros")
    PHP_CHECK_GCC_ARG(-Wno-sign-compare,                _MAINTAINER_CFLAGS="$_MAINTAINER_CFLAGS -Wno-sign-compare")
    PHP_CHECK_GCC_ARG(-fstack-protector,                _MAINTAINER_CFLAGS="$_MAINTAINER_CFLAGS -fstack-protector")
    PHP_CHECK_GCC_ARG(-fno-exceptions,                  _MAINTAINER_CFLAGS="$_MAINTAINER_CFLAGS -fno-exceptions")
    PHP_CHECK_GCC_ARG(-Wformat-security,                _MAINTAINER_CFLAGS="$_MAINTAINER_CFLAGS -Wformat-security")
    PHP_CHECK_GCC_ARG(-Wformat-nonliteral,              _MAINTAINER_CFLAGS="$_MAINTAINER_CFLAGS -Wformat-nonliteral")
    PHP_CHECK_GCC_ARG(-Winit-self,                      _MAINTAINER_CFLAGS="$_MAINTAINER_CFLAGS -Winit-self")
    PHP_CHECK_GCC_ARG(-Wwrite-strings,                  _MAINTAINER_CFLAGS="$_MAINTAINER_CFLAGS -Wwrite-strings")
    PHP_CHECK_GCC_ARG(-Wenum-compare,                   _MAINTAINER_CFLAGS="$_MAINTAINER_CFLAGS -Wenum-compare")
    PHP_CHECK_GCC_ARG(-Wempty-body,                     _MAINTAINER_CFLAGS="$_MAINTAINER_CFLAGS -Wempty-body")
    PHP_CHECK_GCC_ARG(-Wparentheses,                    _MAINTAINER_CFLAGS="$_MAINTAINER_CFLAGS -Wparentheses")
    PHP_CHECK_GCC_ARG(-Wdeclaration-after-statement,    _MAINTAINER_CFLAGS="$_MAINTAINER_CFLAGS -Wdeclaration-after-statement")
    PHP_CHECK_GCC_ARG(-Werror,                          _MAINTAINER_CFLAGS="$_MAINTAINER_CFLAGS -Werror")
    PHP_CHECK_GCC_ARG(-Wextra,                          _MAINTAINER_CFLAGS="$_MAINTAINER_CFLAGS -Wextra")
    PHP_CHECK_GCC_ARG(-Wno-unused-parameter,            _MAINTAINER_CFLAGS="$_MAINTAINER_CFLAGS -Wno-unused-parameter")
    PHP_CHECK_GCC_ARG(-Wno-unused-but-set-variable,     _MAINTAINER_CFLAGS="$_MAINTAINER_CFLAGS -Wno-unused-but-set-variable")
    PHP_CHECK_GCC_ARG(-Wno-missing-field-initializers,  _MAINTAINER_CFLAGS="$_MAINTAINER_CFLAGS -Wno-missing-field-initializers")

    MAINTAINER_CFLAGS="$_MAINTAINER_CFLAGS"
    STD_CFLAGS="-g -O0 -Wall"
  fi


  PHP_ARG_ENABLE(coverage, whether to enable code coverage,
  [  --enable-coverage Enable developer code coverage information],, no)

  if test "$PHP_COVERAGE" = "yes"; then
      PHP_CHECK_GCC_ARG(-fprofile-arcs,                     COVERAGE_CFLAGS="$COVERAGE_CFLAGS -fprofile-arcs")
      PHP_CHECK_GCC_ARG(-ftest-coverage,                    COVERAGE_CFLAGS="$COVERAGE_CFLAGS -ftest-coverage")
      EXTRA_LDFLAGS="$COVERAGE_CFLAGS"
  fi

  PHP_MONGODB_CFLAGS="$STD_CFLAGS $MAINTAINER_CFLAGS $COVERAGE_CFLAGS"

  PHP_MONGODB_SOURCES="\
    php_phongo.c \
    phongo_compat.c \
    src/bson.c \
    src/BSON/Binary.c \
    src/BSON/Decimal128.c \
    src/BSON/Javascript.c \
    src/BSON/MaxKey.c \
    src/BSON/MinKey.c \
    src/BSON/ObjectID.c \
    src/BSON/Persistable.c \
    src/BSON/Regex.c \
    src/BSON/Serializable.c \
    src/BSON/Timestamp.c \
    src/BSON/Type.c \
    src/BSON/Unserializable.c \
    src/BSON/UTCDateTime.c \
    src/MongoDB/BulkWrite.c \
    src/MongoDB/Command.c \
    src/MongoDB/Cursor.c \
    src/MongoDB/CursorId.c \
    src/MongoDB/Manager.c \
    src/MongoDB/Query.c \
    src/MongoDB/ReadConcern.c \
    src/MongoDB/ReadPreference.c \
    src/MongoDB/Server.c \
    src/MongoDB/WriteConcern.c \
    src/MongoDB/WriteConcernError.c \
    src/MongoDB/WriteError.c \
    src/MongoDB/WriteResult.c \
    src/MongoDB/Exception/AuthenticationException.c \
    src/MongoDB/Exception/BulkWriteException.c \
    src/MongoDB/Exception/ConnectionException.c \
    src/MongoDB/Exception/ConnectionTimeoutException.c \
    src/MongoDB/Exception/Exception.c \
    src/MongoDB/Exception/ExecutionTimeoutException.c \
    src/MongoDB/Exception/InvalidArgumentException.c \
    src/MongoDB/Exception/LogicException.c \
    src/MongoDB/Exception/RuntimeException.c \
    src/MongoDB/Exception/SSLConnectionException.c \
    src/MongoDB/Exception/UnexpectedValueException.c \
    src/MongoDB/Exception/WriteException.c \
  "

  PHP_ARG_WITH(libbson, whether to use system libbson,
  [  --with-libbson             Use system libbson], no, no)
  PHP_ARG_WITH(libmongoc, whether to use system libmongoc,
  [  --with-libmongoc           Use system libmongoc], no, no)

  if test "$PHP_LIBBSON" != "no"; then
    if test "$PHP_LIBMONGOC" == "no"; then
      AC_MSG_ERROR(Cannot use system libbson and bundled libmongoc)
    fi

    AC_PATH_PROG(PKG_CONFIG, pkg-config, no)
    AC_MSG_CHECKING(for libbson)
    if test -x "$PKG_CONFIG" && $PKG_CONFIG --exists libbson-1.0; then
      if $PKG_CONFIG libbson-1.0 --atleast-version 1.5.0; then
        LIBBSON_INC=`$PKG_CONFIG libbson-1.0 --cflags`
        LIBBSON_LIB=`$PKG_CONFIG libbson-1.0 --libs`
        LIBBSON_VER=`$PKG_CONFIG libbson-1.0 --modversion`
        AC_MSG_RESULT(version $LIBBSON_VER found)
      else
        AC_MSG_ERROR(system libbson must be upgraded to version >= 1.5.0)
      fi
    else
      AC_MSG_ERROR(pkgconfig and libbson must be installed)
    fi
    PHP_EVAL_INCLINE($LIBBSON_INC)
    PHP_EVAL_LIBLINE($LIBBSON_LIB, MONGODB_SHARED_LIBADD)
    AC_DEFINE(HAVE_SYSTEM_LIBBSON, 1, [Use system libbson])
  else
    PHP_MONGODB_BSON_CFLAGS="$STD_CFLAGS -DBSON_COMPILATION"

    # Generated with: find src/libbson/src/bson -name '*.c' -print0 | cut -sz -d / -f 5- | sort -z | tr '\000' ' '
    PHP_MONGODB_BSON_SOURCES="bcon.c bson-atomic.c bson.c bson-clock.c bson-context.c bson-decimal128.c bson-error.c bson-iso8601.c bson-iter.c bson-json.c bson-keys.c bson-md5.c bson-memory.c bson-oid.c bson-reader.c bson-string.c bson-timegm.c bson-utf8.c bson-value.c bson-version-functions.c bson-writer.c"

    # Generated with: find src/libbson/src/yajl -name '*.c' -print0 | cut -sz -d / -f 5- | sort -z | tr '\000' ' '
    PHP_MONGODB_YAJL_SOURCES="yajl_alloc.c yajl_buf.c yajl.c yajl_encode.c yajl_gen.c yajl_lex.c yajl_parser.c yajl_tree.c yajl_version.c"

    PHP_ADD_SOURCES_X(PHP_EXT_DIR(mongodb)[src/libbson/src/bson], $PHP_MONGODB_BSON_SOURCES, $PHP_MONGODB_BSON_CFLAGS, shared_objects_mongodb, yes)
    PHP_ADD_SOURCES_X(PHP_EXT_DIR(mongodb)[src/libbson/src/yajl], $PHP_MONGODB_YAJL_SOURCES, $PHP_MONGODB_BSON_CFLAGS, shared_objects_mongodb, yes)
  fi

  AC_MSG_CHECKING(configuring libmongoc)
  AC_MSG_RESULT(...)

  if test "$PHP_LIBMONGOC" != "no"; then
    if test "$PHP_LIBBSON" == "no"; then
      AC_MSG_ERROR(Cannot use system libmongoc and bundled libbson)
    fi

    AC_PATH_PROG(PKG_CONFIG, pkg-config, no)
    AC_MSG_CHECKING(for libmongoc)
    if test -x "$PKG_CONFIG" && $PKG_CONFIG --exists libmongoc-1.0; then
      if $PKG_CONFIG libmongoc-1.0 --atleast-version 1.5.0; then
        LIBMONGOC_INC=`$PKG_CONFIG libmongoc-1.0 --cflags`
        LIBMONGOC_LIB=`$PKG_CONFIG libmongoc-1.0 --libs`
        LIBMONGOC_VER=`$PKG_CONFIG libmongoc-1.0 --modversion`
        AC_MSG_RESULT(version $LIBMONGOC_VER found)

      else
        AC_MSG_ERROR(system libmongoc must be upgraded to version >= 1.5.0)
      fi
    else
      AC_MSG_ERROR(pkgconfig and mongoc must be installed)
    fi
    PHP_EVAL_INCLINE($LIBMONGOC_INC)
    PHP_EVAL_LIBLINE($LIBMONGOC_LIB, MONGODB_SHARED_LIBADD)
    AC_DEFINE(HAVE_SYSTEM_LIBMONGOC, 1, [Use system libmongoc])
  else
    PHP_MONGODB_MONGOC_CFLAGS="$STD_CFLAGS -DMONGOC_COMPILATION -DMONGOC_TRACE"

    # Generated with: find src/libmongoc/src/mongoc -name '*.c' -print0 | cut -sz -d / -f 4- | sort -z | tr '\000' ' '
    PHP_MONGODB_MONGOC_SOURCES="mongoc-apm.c mongoc-array.c mongoc-async.c mongoc-async-cmd.c mongoc-b64.c mongoc-buffer.c mongoc-bulk-operation.c mongoc-client.c mongoc-client-pool.c mongoc-cluster.c mongoc-collection.c mongoc-counters.c mongoc-crypto.c mongoc-crypto-cng.c mongoc-crypto-common-crypto.c mongoc-crypto-openssl.c mongoc-cursor-array.c mongoc-cursor.c mongoc-cursor-cursorid.c mongoc-cursor-transform.c mongoc-database.c mongoc-find-and-modify.c mongoc-gridfs.c mongoc-gridfs-file.c mongoc-gridfs-file-list.c mongoc-gridfs-file-page.c mongoc-handshake.c mongoc-host-list.c mongoc-index.c mongoc-init.c mongoc-libressl.c mongoc-linux-distro-scanner.c mongoc-list.c mongoc-log.c mongoc-matcher.c mongoc-matcher-op.c mongoc-memcmp.c mongoc-opcode.c mongoc-openssl.c mongoc-queue.c mongoc-rand-cng.c mongoc-rand-common-crypto.c mongoc-rand-openssl.c mongoc-read-concern.c mongoc-read-prefs.c mongoc-rpc.c mongoc-sasl.c mongoc-scram.c mongoc-secure-channel.c mongoc-secure-transport.c mongoc-server-description.c mongoc-server-stream.c mongoc-set.c mongoc-socket.c mongoc-ssl.c mongoc-stream-buffered.c mongoc-stream.c mongoc-stream-file.c mongoc-stream-gridfs.c mongoc-stream-socket.c mongoc-stream-tls.c mongoc-stream-tls-libressl.c mongoc-stream-tls-openssl-bio.c mongoc-stream-tls-openssl.c mongoc-stream-tls-secure-channel.c mongoc-stream-tls-secure-transport.c mongoc-topology.c mongoc-topology-description-apm.c mongoc-topology-description.c mongoc-topology-scanner.c mongoc-uri.c mongoc-util.c mongoc-version-functions.c mongoc-write-command.c mongoc-write-concern.c"

    PHP_ADD_SOURCES_X(PHP_EXT_DIR(mongodb)[src/libmongoc/src/mongoc], $PHP_MONGODB_MONGOC_SOURCES, $PHP_MONGODB_MONGOC_CFLAGS, shared_objects_mongodb, yes)

    AC_SUBST(MONGOC_ENABLE_CRYPTO, 0)
    AC_SUBST(MONGOC_ENABLE_SSL, 0)
    AC_SUBST(MONGOC_ENABLE_CRYPTO_LIBCRYPTO, 0)
    AC_SUBST(MONGOC_ENABLE_SSL_OPENSSL, 0)

    PHP_SETUP_OPENSSL(MONGODB_SHARED_LIBADD, [
      AC_SUBST(MONGOC_ENABLE_CRYPTO, 1)
      AC_SUBST(MONGOC_ENABLE_SSL, 1)
      AC_SUBST(MONGOC_ENABLE_CRYPTO_LIBCRYPTO, 1)
      AC_SUBST(MONGOC_ENABLE_SSL_OPENSSL, 1)
    ])

    if test "$PHP_SYSTEM_CIPHERS" != "no"; then
      AC_SUBST(MONGOC_ENABLE_CRYPTO_SYSTEM_PROFILE, 1)
    else
      AC_SUBST(MONGOC_ENABLE_CRYPTO_SYSTEM_PROFILE, 0)
    fi

    dnl TODO: Support building with Secure Transport on OSX
    AC_SUBST(MONGOC_ENABLE_SSL_SECURE_TRANSPORT, 0)
    AC_SUBST(MONGOC_ENABLE_CRYPTO_COMMON_CRYPTO, 0)

    dnl Secure Channel only applies to Windows
    AC_SUBST(MONGOC_ENABLE_SSL_SECURE_CHANNEL, 0)
    AC_SUBST(MONGOC_ENABLE_CRYPTO_CNG, 0)

    AC_SUBST(MONGOC_ENABLE_SSL_LIBRESSL, 0)

    AC_SUBST(MONGOC_NO_AUTOMATIC_GLOBALS, 1)
  fi


PHP_ARG_WITH(mongodb-sasl, for Cyrus SASL support,
[  --with-mongodb-sasl[=DIR]     mongodb: Include Cyrus SASL support], auto, no)

if test "$PHP_MONGODB_SASL" != "no"; then
  AC_MSG_CHECKING(for SASL)
  for i in $PHP_MONGODB_SASL /usr /usr/local; do
    if test -f $i/include/sasl/sasl.h; then
      MONGODB_SASL_DIR=$i
      AC_MSG_RESULT(found in $i)
      break
    fi
  done

  if test -z "$MONGODB_SASL_DIR"; then
    AC_MSG_RESULT(not found)
    if test "$PHP_MONGODB_SASL" != "auto"; then
      AC_MSG_ERROR([sasl.h not found!])
    else
      AC_SUBST(MONGOC_ENABLE_SASL, 0)
      AC_SUBST(MONGOC_HAVE_SASL_CLIENT_DONE, 0)
    fi
  else

    PHP_CHECK_LIBRARY(sasl2, sasl_version,
    [
      PHP_ADD_INCLUDE($MONGODB_SASL_DIR)
      PHP_ADD_LIBRARY_WITH_PATH(sasl2, $MONGODB_SASL_DIR/$PHP_LIBDIR, MONGODB_SHARED_LIBADD)
      AC_SUBST(MONGOC_ENABLE_SASL, 1)
    ], [
      if test "$MONGODB_SASL" != "auto"; then
        AC_MSG_ERROR([MongoDB SASL check failed. Please check config.log for more information.])
      fi
      AC_SUBST(MONGOC_ENABLE_SASL, 0)
    ], [
      -L$MONGODB_SASL_DIR/$PHP_LIBDIR
    ])

    PHP_CHECK_LIBRARY(sasl2, sasl_client_done,
    [
      AC_SUBST(MONGOC_HAVE_SASL_CLIENT_DONE, 1)
    ], [
      AC_SUBST(MONGOC_HAVE_SASL_CLIENT_DONE, 0)
    ])
  fi
else
  AC_SUBST(MONGOC_ENABLE_SASL, 0)
  AC_SUBST(MONGOC_HAVE_SASL_CLIENT_DONE, 0)
fi

  m4_include(src/libmongoc/build/autotools/WeakSymbols.m4)
  m4_include(src/libmongoc/build/autotools/m4/ax_pthread.m4)
  AX_PTHREAD

  AC_CHECK_FUNCS([shm_open], [SHM_LIB=], [AC_CHECK_LIB([rt], [shm_open], [SHM_LIB=-lrt], [SHM_LIB=])])
  MONGODB_SHARED_LIBADD="$MONGODB_SHARED_LIBADD $SHM_LIB"

  EXTRA_CFLAGS="$PTHREAD_CFLAGS $SASL_CFLAGS"
  PHP_SUBST(EXTRA_CFLAGS)
  PHP_SUBST(EXTRA_LDFLAGS)

  MONGODB_SHARED_LIBADD="$MONGODB_SHARED_LIBADD $PTHREAD_LIBS $SASL_LIBS"
  PHP_SUBST(MONGODB_SHARED_LIBADD)

  PHP_NEW_EXTENSION(mongodb, $PHP_MONGODB_SOURCES, $ext_shared,, $PHP_MONGODB_CFLAGS)
  PHP_ADD_EXTENSION_DEP(mongodb, date)
  PHP_ADD_EXTENSION_DEP(mongodb, json)
  PHP_ADD_EXTENSION_DEP(mongodb, spl)
  PHP_ADD_EXTENSION_DEP(mongodb, standard)

  PHP_ADD_INCLUDE([$ext_srcdir/src/BSON/])
  PHP_ADD_INCLUDE([$ext_srcdir/src/MongoDB/])
  PHP_ADD_INCLUDE([$ext_srcdir/src/MongoDB/Exception/])
  PHP_ADD_INCLUDE([$ext_srcdir/src/contrib/])
  PHP_ADD_BUILD_DIR([$ext_builddir/src/BSON/])
  PHP_ADD_BUILD_DIR([$ext_builddir/src/MongoDB/])
  PHP_ADD_BUILD_DIR([$ext_builddir/src/MongoDB/Exception/])
  PHP_ADD_BUILD_DIR([$ext_builddir/src/contrib/])
  if test "$PHP_LIBMONGOC" == "no"; then
    PHP_ADD_INCLUDE([$ext_srcdir/src/libmongoc/src/mongoc/])
    PHP_ADD_BUILD_DIR([$ext_builddir/src/libmongoc/src/mongoc/])
  fi
  if test "$PHP_LIBBSON" == "no"; then
    m4_include(src/libbson/build/autotools/CheckAtomics.m4)
    m4_include(src/libbson/build/autotools/FindDependencies.m4)
    m4_include(src/libbson/build/autotools/m4/ac_compile_check_sizeof.m4)
    m4_include(src/libbson/build/autotools/m4/ac_create_stdint_h.m4)
    AC_CREATE_STDINT_H([$srcdir/src/libbson/src/bson/bson-stdint.h])
    PHP_ADD_INCLUDE([$ext_srcdir/src/libbson/src/])
    PHP_ADD_INCLUDE([$ext_srcdir/src/libbson/src/yajl/])
    PHP_ADD_INCLUDE([$ext_srcdir/src/libbson/src/bson/])
    PHP_ADD_BUILD_DIR([$ext_builddir/src/libbson/src/])
    PHP_ADD_BUILD_DIR([$ext_builddir/src/libbson/src/yajl/])
    PHP_ADD_BUILD_DIR([$ext_builddir/src/libbson/src/bson/])
  fi

  PHP_BSON_BIGENDIAN
  AC_HEADER_STDBOOL

  AC_SUBST(BSON_EXTRA_ALIGN, 0)
  AC_SUBST(BSON_HAVE_DECIMAL128, 0)

  if test "$ac_cv_header_stdbool_h" = "yes"; then
    AC_SUBST(BSON_HAVE_STDBOOL_H, 1)
  else
    AC_SUBST(BSON_HAVE_STDBOOL_H, 0)
  fi

  AC_SUBST(BSON_OS, 1)

  PHP_BSON_CLOCK
  AC_CHECK_FUNC(strnlen,ac_cv_func_strnlen=yes,ac_cv_func_strnlen=no)
  if test "$ac_cv_func_strnlen" = "yes"; then
    AC_SUBST(BSON_HAVE_STRNLEN, 1)
  else
    AC_SUBST(BSON_HAVE_STRNLEN, 0)
  fi

  AC_CHECK_FUNC(snprintf,ac_cv_func_snprintf=yes,ac_cv_func_snprintf=no)
  if test "$ac_cv_func_snprintf" = "yes"; then
    AC_SUBST(BSON_HAVE_SNPRINTF, 1)
  else
    AC_SUBST(BSON_HAVE_SNPRINTF, 0)
  fi

  if test "$PHP_LIBMONGOC" == "no"; then
    backup_srcdir=${srcdir}
    srcdir=${srcdir}/src/libmongoc/
    m4_include(src/libmongoc/build/autotools/Versions.m4)
    srcdir=${backup_srcdir}
    MONGOC_API_VERSION=1.0
    AC_SUBST(MONGOC_MAJOR_VERSION)
    AC_SUBST(MONGOC_MINOR_VERSION)
    AC_SUBST(MONGOC_MICRO_VERSION)
    AC_SUBST(MONGOC_API_VERSION)
    AC_SUBST(MONGOC_VERSION)
    AC_OUTPUT($srcdir/src/libmongoc/src/mongoc/mongoc-config.h)
    AC_OUTPUT($srcdir/src/libmongoc/src/mongoc/mongoc-version.h)
  fi
  if test "$PHP_LIBBSON" == "no"; then
    backup_srcdir=${srcdir}
    srcdir=${srcdir}/src/libbson/
    m4_include(src/libbson/build/autotools/Versions.m4)
    srcdir=${backup_srcdir}
    BSON_API_VERSION=1.0
    AC_SUBST(BSON_MAJOR_VERSION)
    AC_SUBST(BSON_MINOR_VERSION)
    AC_SUBST(BSON_MICRO_VERSION)
    AC_SUBST(BSON_API_VERSION)
    AC_SUBST(BSON_VERSION)
    AC_OUTPUT($srcdir/src/libbson/src/bson/bson-config.h)
    AC_OUTPUT($srcdir/src/libbson/src/bson/bson-version.h)
  fi

  dnl This must come after PHP_NEW_EXTENSION, otherwise the srcdir won't be set
  PHP_ADD_MAKEFILE_FRAGMENT

AC_CONFIG_COMMANDS_POST([echo "
mongodb was configured with the following options:

Build configuration:
  CFLAGS                                           : $CFLAGS
  Extra CFLAGS                                     : $STD_CFLAGS $EXTRA_CFLAGS
  Developers flags (slow)                          : $MAINTAINER_CFLAGS
  Code Coverage flags (extra slow)                 : $COVERAGE_CFLAGS
  System mongoc                                    : $PHP_LIBMONGOC
  System libbson                                   : $PHP_LIBBSON
  LDFLAGS                                          : $LDFLAGS
  EXTRA_LDFLAGS                                    : $EXTRA_LDFLAGS
  MONGODB_SHARED_LIBADD                            : $MONGODB_SHARED_LIBADD

Please submit bugreports at:
  https://jira.mongodb.org/browse/PHPC

"])
fi

dnl: vim: et sw=2
