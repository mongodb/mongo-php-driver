dnl config.m4 for extension phongo
PHP_ARG_ENABLE(phongo, whether to enable phongo support,
[  --enable-phongo           Enable phongo support])
PHP_ARG_WITH(openssl-dir, OpenSSL dir for phongo,
[  --with-openssl-dir[=DIR]  openssl install prefix], yes, no)


AC_DEFUN([MONGOC_DEFINE_RESET],[
  [echo "" > $srcdir/src/libmongoc/src/mongoc/mongoc-config.h]
])
AC_DEFUN([BSON_DEFINE_RESET],[
  [echo "" > $srcdir/src/libbson/src/bson/bson-config.h]
])
dnl borrowed from PHP acinclude.m4
AC_DEFUN([BSON_DEFINE],[
  [echo "#define ]$1[]ifelse([$2],,[ 1],[ $2])[" >> $srcdir/src/libbson/src/bson/bson-config.h]
])
AC_DEFUN([MONGOC_DEFINE],[
  [echo "#define ]$1[]ifelse([$2],,[ 1],[ $2])[" >> $srcdir/src/libmongoc/src/mongoc/mongoc-config.h]
])
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
    BSON_DEFINE([BSON_BYTE_ORDER], 4321)
  else
    BSON_DEFINE([BSON_BYTE_ORDER], 1234)
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
    BSON_DEFINE([BSON_HAVE_CLOCK_GETTIME], 1)
  fi
])

MONGOC_SYMBOL_SUFFIX="priv"

if test "$PHONGO" != "no"; then
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


    MAINTAINER_CFLAGS="-Wextra $_MAINTAINER_CFLAGS  -Wno-unused-parameter -Wno-unused-but-set-variable -Wno-missing-field-initializers"
    STD_CFLAGS="-g -O0 -Wall"
    dnl EXTRA_LDFLAGS="-Wl,--no-undefined"
  fi


  PHP_ARG_ENABLE(coverage, whether to enable code coverage,
  [  --enable-coverage Enable developer code coverage information],, no)

  if test "$PHP_COVERAGE" = "yes"; then
      PHP_CHECK_GCC_ARG(-fprofile-arcs,                     COVERAGE_CFLAGS="$COVERAGE_CFLAGS -fprofile-arcs")
      PHP_CHECK_GCC_ARG(-ftest-coverage,                    COVERAGE_CFLAGS="$COVERAGE_CFLAGS -ftest-coverage")
      EXTRA_LDFLAGS="$COVERAGE_CFLAGS"
  fi

  PHONGO_BSON="\
      src/bson.c \
  ";
  PHONGO_BSON_CLASSES="\
    src/BSON/Type.c \
    src/BSON/Unserializable.c \
    src/BSON/Serializable.c \
    src/BSON/Persistable.c \
    src/BSON/Binary.c \
    src/BSON/Javascript.c \
    src/BSON/MaxKey.c \
    src/BSON/MinKey.c \
    src/BSON/ObjectID.c \
    src/BSON/Regex.c \
    src/BSON/Timestamp.c \
    src/BSON/UTCDatetime.c \
  ";
  PHONGO_ROOT="\
      php_phongo.c \
  ";
  PHONGO_MONGODB_CLASSES="\
      src/MongoDB/Command.c \
      src/MongoDB/CommandResult.c \
      src/MongoDB/Cursor.c \
      src/MongoDB/CursorId.c \
      src/MongoDB/Manager.c \
      src/MongoDB/Query.c \
      src/MongoDB/QueryResult.c \
      src/MongoDB/ReadPreference.c \
      src/MongoDB/Server.c \
      src/MongoDB/WriteBatch.c \
      src/MongoDB/WriteConcern.c \
      src/MongoDB/WriteConcernError.c \
      src/MongoDB/WriteError.c \
      src/MongoDB/WriteResult.c \
  ";
  PHONGO_MONGODB_EXCEPTIONS="\
      src/MongoDB/Exception.c \
      src/MongoDB/RuntimeException.c \
      src/MongoDB/ConnectionException.c \
      src/MongoDB/AuthenticationException.c \
      src/MongoDB/SSLConnectionException.c \
      src/MongoDB/DuplicateKeyException.c \
      src/MongoDB/WriteException.c \
  ";

  YAJL_SOURCES="\
	yajl_version.c \
	yajl.c \
	yajl_encode.c \
	yajl_lex.c \
	yajl_parser.c \
	yajl_buf.c \
	yajl_tree.c \
	yajl_alloc.c \
	yajl_gen.c
  ";

  BSON_SOURCES="\
	bcon.c \
	bson.c \
	bson-atomic.c \
	bson-clock.c \
	bson-context.c \
	bson-error.c \
	bson-iter.c \
	bson-iso8601.c \
	bson-json.c \
	bson-keys.c \
	bson-md5.c \
	bson-memory.c \
	bson-oid.c \
	bson-reader.c \
	bson-string.c \
	bson-timegm.c \
	bson-utf8.c \
	bson-value.c \
	bson-version.c \
	bson-writer.c
  ";
  MONGOC_SOURCES="\
	mongoc-array.c \
	mongoc-buffer.c \
	mongoc-bulk-operation.c \
	mongoc-client.c \
	mongoc-client-pool.c \
	mongoc-cluster.c \
	mongoc-collection.c \
	mongoc-counters.c \
	mongoc-cursor.c \
	mongoc-cursor-array.c \
	mongoc-cursor-cursorid.c \
	mongoc-database.c \
	mongoc-init.c \
	mongoc-gridfs.c \
	mongoc-gridfs-file.c \
	mongoc-gridfs-file-page.c \
	mongoc-gridfs-file-list.c \
	mongoc-index.c \
	mongoc-list.c \
	mongoc-log.c \
	mongoc-matcher-op.c \
	mongoc-matcher.c \
	mongoc-queue.c \
	mongoc-read-prefs.c \
	mongoc-rpc.c \
	mongoc-socket.c \
	mongoc-stream.c \
	mongoc-stream-buffered.c \
	mongoc-stream-file.c \
	mongoc-stream-gridfs.c \
	mongoc-stream-socket.c \
	mongoc-uri.c \
	mongoc-util.c \
	mongoc-write-command.c \
	mongoc-write-concern.c
  ";

MONGOC_SOURCES_SSL="\
	mongoc-rand.c \
	mongoc-scram.c \
	mongoc-stream-tls.c \
	mongoc-ssl.c
  ";

MONGOC_SOURCES_SASL=mongoc-sasl.c


  if test "$ext_shared" = "no"; then
    PHP_ADD_SOURCES(PHP_EXT_DIR(phongo), $PHONGO_BSON)
    PHP_ADD_SOURCES(PHP_EXT_DIR(phongo), $PHONGO_BSON_CLASSES)
    PHP_ADD_SOURCES(PHP_EXT_DIR(phongo), $PHONGO_MONGODB_CLASSES)
    PHP_ADD_SOURCES(PHP_EXT_DIR(phongo), $PHONGO_MONGODB_EXCEPTIONS)
  else
    PHP_ADD_SOURCES_X(PHP_EXT_DIR(phongo), $PHONGO_BSON,               [$STD_CFLAGS $MAINTAINER_CFLAGS $COVERAGE_CFLAGS], shared_objects_phongo, yes)
    PHP_ADD_SOURCES_X(PHP_EXT_DIR(phongo), $PHONGO_BSON_CLASSES,       [$STD_CFLAGS $MAINTAINER_CFLAGS $COVERAGE_CFLAGS], shared_objects_phongo, yes)
    PHP_ADD_SOURCES_X(PHP_EXT_DIR(phongo), $PHONGO_MONGODB_CLASSES,    [$STD_CFLAGS $MAINTAINER_CFLAGS $COVERAGE_CFLAGS], shared_objects_phongo, yes)
    PHP_ADD_SOURCES_X(PHP_EXT_DIR(phongo), $PHONGO_MONGODB_EXCEPTIONS, [$STD_CFLAGS $MAINTAINER_CFLAGS $COVERAGE_CFLAGS], shared_objects_phongo, yes)
  fi

dnl libmongoc stuff {{{
  AC_MSG_CHECKING(configuring libmongoc)
  AC_MSG_RESULT(...)

  AC_DEFINE(HAVE_MONGOC, 1, [Kinda useless extension without it..])

  CPPFLAGS="$CPPFLAGS -DBSON_COMPILATION -DMONGOC_COMPILATION -DMONGOC_TRACE"

  PHP_ADD_SOURCES_X(PHP_EXT_DIR(phongo)[src/libbson/src/yajl], $YAJL_SOURCES,           [$STD_CFLAGS], shared_objects_phongo, yes)
  PHP_ADD_SOURCES_X(PHP_EXT_DIR(phongo)[src/libbson/src/bson], $BSON_SOURCES,           [$STD_CFLAGS], shared_objects_phongo, yes)
  PHP_ADD_SOURCES_X(PHP_EXT_DIR(phongo)[src/libmongoc/src/mongoc], $MONGOC_SOURCES,     [$STD_CFLAGS], shared_objects_phongo, yes)
  PHP_ADD_SOURCES_X(PHP_EXT_DIR(phongo)[src/libmongoc/src/mongoc], $MONGOC_SOURCES_SSL, [$STD_CFLAGS], shared_objects_phongo, yes)


  MONGOC_DEFINE_RESET
  PHP_SETUP_OPENSSL(PHONGO_SHARED_LIBADD)
  MONGOC_DEFINE([MONGOC_ENABLE_SSL], 1)


PHP_ARG_WITH(phongo-sasl, Build with Cyrus SASL support,
[  --with-phongo-sasl[=DIR]     phongo: Include Cyrus SASL support], auto, yes)

if test "$PHP_PHONGO_SASL" != "no"; then
  AC_MSG_CHECKING(for SASL)
  for i in $PHONGO_SASL /usr /usr/local; do
    if test -f $i/include/sasl/sasl.h; then
      PHONGO_SASL_DIR=$i
      AC_MSG_RESULT(found in $i)
      break
    fi
  done

  if test -z "$PHONGO_SASL_DIR"; then
    AC_MSG_RESULT(not found)
    if test "$PHONGO_SASL" != "auto"; then
      AC_MSG_ERROR([sasl.h not found!])
    fi
  fi

  PHP_CHECK_LIBRARY(sasl2, sasl_version,
  [
    PHP_ADD_INCLUDE($PHONGO_SASL_DIR)
    PHP_ADD_LIBRARY_WITH_PATH(sasl2, $PHONGO_SASL_DIR/$PHP_LIBDIR, PHONGO_SHARED_LIBADD)
    MONGOC_DEFINE(MONGOC_ENABLE_SASL, 1, [MONGO SASL support])
  ], [
    if test "$PHONGO_SASL" != "auto"; then
      AC_MSG_ERROR([MONGO SASL check failed. Please check config.log for more information.])
    fi
  ], [
    -L$PHONGO_SASL_DIR/$PHP_LIBDIR
  ])
fi
  m4_include(src/libmongoc/build/autotools/m4/ax_pthread.m4)
  AX_PTHREAD

  AC_CHECK_FUNCS([shm_open], [SHM_LIB=], [AC_CHECK_LIB([rt], [shm_open], [SHM_LIB=-lrt], [SHM_LIB=])])
  PHONGO_SHARED_LIBADD="$PHONGO_SHARED_LIBADD $SHM_LIB"


  dnl PHP_ADD_LIBRARY_WITH_PATH(bson-1.0, src/libbson/.libs, PHONGO_SHARED_LIBADD)
  dnl PHP_ADD_LIBRARY_WITH_PATH(mongoc-priv, src/libmongoc/.libs, PHONGO_SHARED_LIBADD)
  EXTRA_CFLAGS="$PTHREAD_CFLAGS $SASL_CFLAGS"
  PHP_SUBST(EXTRA_CFLAGS)
  PHP_SUBST(EXTRA_LDFLAGS)

  PHONGO_SHARED_LIBADD="$PHONGO_SHARED_LIBADD $PTHREAD_LIBS $SASL_LIBS"
  PHP_SUBST(PHONGO_SHARED_LIBADD)

dnl }}}

  PHP_NEW_EXTENSION(phongo,    $PHONGO_ROOT, $ext_shared,, [$STD_CFLAGS $MAINTAINER_CFLAGS $COVERAGE_CFLAGS])
  PHP_ADD_EXTENSION_DEP(phongo, spl)

  m4_include(src/libbson/build/autotools/m4/ac_compile_check_sizeof.m4)
  m4_include(src/libbson/build/autotools/m4/ac_create_stdint_h.m4)
  AC_CREATE_STDINT_H([$srcdir/src/libbson/src/bson/bson-stdint.h])

  PHP_ADD_INCLUDE([$ext_srcdir/src/BSON/])
  PHP_ADD_INCLUDE([$ext_srcdir/src/MongoDB/])
  PHP_ADD_INCLUDE([$ext_srcdir/src/libbson/src/])
  PHP_ADD_INCLUDE([$ext_srcdir/src/libbson/src/yajl/])
  PHP_ADD_INCLUDE([$ext_srcdir/src/libbson/src/bson/])
  PHP_ADD_INCLUDE([$ext_srcdir/src/libmongoc/src/mongoc/])
  PHP_ADD_BUILD_DIR([$ext_builddir/src/BSON/])
  PHP_ADD_BUILD_DIR([$ext_builddir/src/MongoDB/])
  PHP_ADD_BUILD_DIR([$ext_builddir/src/libbson/src/])
  PHP_ADD_BUILD_DIR([$ext_builddir/src/libbson/src/yajl/])
  PHP_ADD_BUILD_DIR([$ext_builddir/src/libbson/src/bson/])
  PHP_ADD_BUILD_DIR([$ext_builddir/src/libmongoc/src/mongoc/])

  dnl PHONGO_SHARED_DEPENDENCIES="phongodep"
  dnl PHP_SUBST(PHONGO_SHARED_DEPENDENCIES)

  BSON_DEFINE_RESET

  PHP_BSON_BIGENDIAN
  AC_HEADER_STDBOOL

  if test "$ac_cv_header_stdbool_h" = "yes"; then
    BSON_DEFINE([BSON_HAVE_STDBOOL_H], 1)
  else
    BSON_DEFINE([BSON_HAVE_STDBOOL_H], 0)
  fi

  BSON_DEFINE([BSON_OS], 1)

  PHP_BSON_CLOCK
  AC_CHECK_FUNC(strnlen,ac_cv_func_strnlen=yes,ac_cv_func_strnlen=no)
  if test "$ac_cv_func_strnlen" = "yes"; then
    BSON_DEFINE([BSON_HAVE_STRNLEN], 1)
  fi
  AC_CHECK_FUNC(snprintf,ac_cv_func_snprintf=yes,ac_cv_func_snprintf=no)
  if test "$ac_cv_func_snprintf" = "yes"; then
    BSON_DEFINE([BSON_HAVE_SNPRINTF], 1)
  fi

  dnl This must come after PHP_NEW_EXTENSION, otherwise the srcdir won't be set
  PHP_ADD_MAKEFILE_FRAGMENT

AC_CONFIG_COMMANDS_POST([echo "
phongo was configured with the following options:

Build configuration:
  CFLAGS                                           : $CFLAGS
  Extra CFLAGS                                     : $STD_CFLAGS $EXTRA_CFLAGS
  Developers flags (slow)                          : $MAINTAINER_CFLAGS
  Code Coverage flags (extra slow)                 : $COVERAGE_CFLAGS
  LDFLAGS                                          : $LDFLAGS
  EXTRA_LDFLAGS                                    : $EXTRA_LDFLAGS
  PHONGO_SHARED_LIBADD                             : $PHONGO_SHARED_LIBADD

Please submit bugreports at:
  https://jira.mongodb.org/browse/PHP

"])
fi

dnl: vim: et sw=2
