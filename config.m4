dnl config.m4 for extension mongodb
syscmd("./autogen.sh")

PHP_ARG_ENABLE(mongodb, whether to enable mongodb support,
               [AS_HELP_STRING([--enable-mongodb], [Enable MongoDB support])])

if test "$MONGODB" != "no"; then
  PHP_ARG_ENABLE(developer-flags, whether to enable developer build flags,
                 [AS_HELP_STRING([--developer-flags], [Enable developer build flags])], no, no)

  PHP_ARG_WITH(libbson, whether to use system libbson instead of bundled version,
               [AS_HELP_STRING([--with-libbson], [Use system libbson instead of bundled version])], no, no)

  PHP_ARG_WITH(libmongoc, whether to use system libmongoc instead of bundled version,
               [AS_HELP_STRING([--with-libmongoc], [Use system libmongoc instead of bundled version])], no, no)

  if test "$PHP_DEVELOPER_FLAGS" = "yes"; then
    dnl Warn about functions which might be candidates for format attributes
    PHP_CHECK_GCC_ARG(-Wmissing-format-attribute,       PHP_MONGODB_DEV_CFLAGS="$PHP_MONGODB_DEV_CFLAGS -Wmissing-format-attribute")
    dnl Avoid duplicating values for an enum
    PHP_CHECK_GCC_ARG(-Wduplicate-enum,                 PHP_MONGODB_DEV_CFLAGS="$PHP_MONGODB_DEV_CFLAGS -Wduplicate-enum")
    dnl Warns on mismatches between #ifndef and #define header guards
    PHP_CHECK_GCC_ARG(-Wheader-guard,                   PHP_MONGODB_DEV_CFLAGS="$PHP_MONGODB_DEV_CFLAGS -Wheader-guard")
    dnl logical not of a non-boolean expression
    PHP_CHECK_GCC_ARG(-Wlogical-not-parentheses,        PHP_MONGODB_DEV_CFLAGS="$PHP_MONGODB_DEV_CFLAGS -Wlogical-not-parentheses")
    dnl Warn about suspicious uses of logical operators in expressions
    PHP_CHECK_GCC_ARG(-Wlogical-op,                     PHP_MONGODB_DEV_CFLAGS="$PHP_MONGODB_DEV_CFLAGS -Wlogical-op")
    dnl memory error detector.
    dnl FIXME: -fsanitize=address,undefined for clang. The PHP_CHECK_GCC_ARG macro isn't happy about that string :(
    PHP_CHECK_GCC_ARG(-fsanitize-address,               PHP_MONGODB_DEV_CFLAGS="$PHP_MONGODB_DEV_CFLAGS -fsanitize-address")
    dnl Enable frame debugging
    PHP_CHECK_GCC_ARG(-fno-omit-frame-pointer,          PHP_MONGODB_DEV_CFLAGS="$PHP_MONGODB_DEV_CFLAGS -fno-omit-frame-pointer")
    dnl Make sure we don't optimize calls
    PHP_CHECK_GCC_ARG(-fno-optimize-sibling-calls,      PHP_MONGODB_DEV_CFLAGS="$PHP_MONGODB_DEV_CFLAGS -fno-optimize-sibling-calls")
    PHP_CHECK_GCC_ARG(-Wlogical-op-parentheses,         PHP_MONGODB_DEV_CFLAGS="$PHP_MONGODB_DEV_CFLAGS -Wlogical-op-parentheses")
    PHP_CHECK_GCC_ARG(-Wbool-conversion,                PHP_MONGODB_DEV_CFLAGS="$PHP_MONGODB_DEV_CFLAGS -Wbool-conversion")
    PHP_CHECK_GCC_ARG(-Wloop-analysis,                  PHP_MONGODB_DEV_CFLAGS="$PHP_MONGODB_DEV_CFLAGS -Wloop-analysis")
    PHP_CHECK_GCC_ARG(-Wsizeof-array-argument,          PHP_MONGODB_DEV_CFLAGS="$PHP_MONGODB_DEV_CFLAGS -Wsizeof-array-argument")
    PHP_CHECK_GCC_ARG(-Wstring-conversion,              PHP_MONGODB_DEV_CFLAGS="$PHP_MONGODB_DEV_CFLAGS -Wstring-conversion")
    PHP_CHECK_GCC_ARG(-Wno-variadic-macros,             PHP_MONGODB_DEV_CFLAGS="$PHP_MONGODB_DEV_CFLAGS -Wno-variadic-macros")
    PHP_CHECK_GCC_ARG(-Wno-sign-compare,                PHP_MONGODB_DEV_CFLAGS="$PHP_MONGODB_DEV_CFLAGS -Wno-sign-compare")
    PHP_CHECK_GCC_ARG(-fstack-protector,                PHP_MONGODB_DEV_CFLAGS="$PHP_MONGODB_DEV_CFLAGS -fstack-protector")
    PHP_CHECK_GCC_ARG(-fno-exceptions,                  PHP_MONGODB_DEV_CFLAGS="$PHP_MONGODB_DEV_CFLAGS -fno-exceptions")
    PHP_CHECK_GCC_ARG(-Wformat-security,                PHP_MONGODB_DEV_CFLAGS="$PHP_MONGODB_DEV_CFLAGS -Wformat-security")
    PHP_CHECK_GCC_ARG(-Wformat-nonliteral,              PHP_MONGODB_DEV_CFLAGS="$PHP_MONGODB_DEV_CFLAGS -Wformat-nonliteral")
    PHP_CHECK_GCC_ARG(-Winit-self,                      PHP_MONGODB_DEV_CFLAGS="$PHP_MONGODB_DEV_CFLAGS -Winit-self")
    PHP_CHECK_GCC_ARG(-Wwrite-strings,                  PHP_MONGODB_DEV_CFLAGS="$PHP_MONGODB_DEV_CFLAGS -Wwrite-strings")
    PHP_CHECK_GCC_ARG(-Wenum-compare,                   PHP_MONGODB_DEV_CFLAGS="$PHP_MONGODB_DEV_CFLAGS -Wenum-compare")
    PHP_CHECK_GCC_ARG(-Wempty-body,                     PHP_MONGODB_DEV_CFLAGS="$PHP_MONGODB_DEV_CFLAGS -Wempty-body")
    PHP_CHECK_GCC_ARG(-Wparentheses,                    PHP_MONGODB_DEV_CFLAGS="$PHP_MONGODB_DEV_CFLAGS -Wparentheses")
    PHP_CHECK_GCC_ARG(-Wdeclaration-after-statement,    PHP_MONGODB_DEV_CFLAGS="$PHP_MONGODB_DEV_CFLAGS -Wdeclaration-after-statement")
    PHP_CHECK_GCC_ARG(-Werror,                          PHP_MONGODB_DEV_CFLAGS="$PHP_MONGODB_DEV_CFLAGS -Werror")
    PHP_CHECK_GCC_ARG(-Wextra,                          PHP_MONGODB_DEV_CFLAGS="$PHP_MONGODB_DEV_CFLAGS -Wextra")
    PHP_CHECK_GCC_ARG(-Wno-unused-parameter,            PHP_MONGODB_DEV_CFLAGS="$PHP_MONGODB_DEV_CFLAGS -Wno-unused-parameter")
    PHP_CHECK_GCC_ARG(-Wno-unused-but-set-variable,     PHP_MONGODB_DEV_CFLAGS="$PHP_MONGODB_DEV_CFLAGS -Wno-unused-but-set-variable")
    PHP_CHECK_GCC_ARG(-Wno-missing-field-initializers,  PHP_MONGODB_DEV_CFLAGS="$PHP_MONGODB_DEV_CFLAGS -Wno-missing-field-initializers")
    PHP_CHECK_GCC_ARG(-Wall,                            PHP_MONGODB_DEV_CFLAGS="$PHP_MONGODB_DEV_CFLAGS -Wall")

    dnl Add debug symbols and disable optimization
    STD_CFLAGS="-g -O0"
  fi

  PHP_ARG_ENABLE(coverage, whether to enable code coverage,
                 [AS_HELP_STRING([--enable-coverage], [Enable developer code coverage information])],
                 no, no)

  if test "$PHP_COVERAGE" = "yes"; then
      PHP_CHECK_GCC_ARG(-fprofile-arcs,  PHP_MONGODB_COVERAGE_CFLAGS="$PHP_MONGODB_COVERAGE_CFLAGS -fprofile-arcs")
      PHP_CHECK_GCC_ARG(-ftest-coverage, PHP_MONGODB_COVERAGE_CFLAGS="$PHP_MONGODB_COVERAGE_CFLAGS -ftest-coverage")
      EXTRA_LDFLAGS="$COVERAGE_CFLAGS"
  fi

  PHP_MONGODB_CFLAGS="$STD_CFLAGS $PHP_MONGODB_DEV_CFLAGS $PHP_MONGODB_COVERAGE_CFLAGS"

  PHP_MONGODB_SOURCES="\
    php_phongo.c \
    phongo_compat.c \
    src/bson.c \
    src/BSON/Type.c \
    src/BSON/Unserializable.c \
    src/BSON/Serializable.c \
    src/BSON/Persistable.c \
    src/BSON/Binary.c \
    src/BSON/Decimal128.c \
    src/BSON/Javascript.c \
    src/BSON/MaxKey.c \
    src/BSON/MinKey.c \
    src/BSON/ObjectID.c \
    src/BSON/Regex.c \
    src/BSON/Timestamp.c \
    src/BSON/UTCDateTime.c \
    src/MongoDB/Command.c \
    src/MongoDB/Cursor.c \
    src/MongoDB/CursorId.c \
    src/MongoDB/Manager.c \
    src/MongoDB/Query.c \
    src/MongoDB/ReadConcern.c \
    src/MongoDB/ReadPreference.c \
    src/MongoDB/Server.c \
    src/MongoDB/BulkWrite.c \
    src/MongoDB/WriteConcern.c \
    src/MongoDB/WriteConcernError.c \
    src/MongoDB/WriteError.c \
    src/MongoDB/WriteResult.c \
    src/MongoDB/Exception/Exception.c \
    src/MongoDB/Exception/LogicException.c \
    src/MongoDB/Exception/RuntimeException.c \
    src/MongoDB/Exception/UnexpectedValueException.c \
    src/MongoDB/Exception/InvalidArgumentException.c \
    src/MongoDB/Exception/ConnectionException.c \
    src/MongoDB/Exception/AuthenticationException.c \
    src/MongoDB/Exception/SSLConnectionException.c \
    src/MongoDB/Exception/ExecutionTimeoutException.c \
    src/MongoDB/Exception/ConnectionTimeoutException.c \
    src/MongoDB/Exception/WriteException.c \
    src/MongoDB/Exception/BulkWriteException.c \
  ";

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
  fi

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
  fi

  if test "$PHP_LIBMONGOC" == "no"; then
    PHP_ARG_WITH(system-ciphers, whether to use system crypto profile (bundled libmongoc and OpenSSL only),
                 [AS_HELP_STRING([--with-system-ciphers], [Use system crypto profile (bundled libmongoc and OpenSSL only)])], no, no)

    PHP_ARG_ENABLE(debug, whether to include debugging symbols,
                   [AS_HELP_STRING([--enable-debug], [Compile with debugging symbols])], no, no)

    # Generated with: find src/libmongoc/src/libbson/src/bson/*c -printf '%f '
    BSON_SOURCES="bcon.c bson-atomic.c bson.c bson-clock.c bson-context.c bson-decimal128.c bson-error.c bson-iso8601.c bson-iter.c bson-json.c bson-keys.c bson-md5.c bson-memory.c bson-oid.c bson-reader.c bson-string.c bson-timegm.c bson-utf8.c bson-value.c bson-version-functions.c bson-writer.c";

    # Generated with: find src/libmongoc/src/libbson/src/yajl/*c -printf '%f '
    YAJL_SOURCES="yajl_alloc.c yajl_buf.c yajl.c yajl_encode.c yajl_gen.c yajl_lex.c yajl_parser.c yajl_tree.c yajl_version.c";

    # Generated with: find src/libmongoc/src/mongoc/*c -printf '%f '
    MONGOC_SOURCES="mongoc-apm.c mongoc-array.c mongoc-async.c mongoc-async-cmd.c mongoc-b64.c mongoc-buffer.c mongoc-bulk-operation.c mongoc-client.c mongoc-client-pool.c mongoc-cluster.c mongoc-collection.c mongoc-counters.c mongoc-crypto.c mongoc-crypto-cng.c mongoc-crypto-common-crypto.c mongoc-crypto-openssl.c mongoc-cursor-array.c mongoc-cursor.c mongoc-cursor-cursorid.c mongoc-cursor-transform.c mongoc-database.c mongoc-find-and-modify.c mongoc-gridfs.c mongoc-gridfs-file.c mongoc-gridfs-file-list.c mongoc-gridfs-file-page.c mongoc-handshake.c mongoc-host-list.c mongoc-index.c mongoc-init.c mongoc-libressl.c mongoc-linux-distro-scanner.c mongoc-list.c mongoc-log.c mongoc-matcher.c mongoc-matcher-op.c mongoc-memcmp.c mongoc-opcode.c mongoc-openssl.c mongoc-queue.c mongoc-rand-cng.c mongoc-rand-common-crypto.c mongoc-rand-openssl.c mongoc-read-concern.c mongoc-read-prefs.c mongoc-rpc.c mongoc-sasl.c mongoc-scram.c mongoc-secure-channel.c mongoc-secure-transport.c mongoc-server-description.c mongoc-server-stream.c mongoc-set.c mongoc-socket.c mongoc-ssl.c mongoc-stream-buffered.c mongoc-stream.c mongoc-stream-file.c mongoc-stream-gridfs.c mongoc-stream-socket.c mongoc-stream-tls.c mongoc-stream-tls-libressl.c mongoc-stream-tls-openssl-bio.c mongoc-stream-tls-openssl.c mongoc-stream-tls-secure-channel.c mongoc-stream-tls-secure-transport.c mongoc-topology.c mongoc-topology-description-apm.c mongoc-topology-description.c mongoc-topology-scanner.c mongoc-uri.c mongoc-util.c mongoc-version-functions.c mongoc-write-command.c mongoc-write-concern.c";

    PHP_LIBMONGOC_CFLAGS="$STD_CFLAGS -DBSON_COMPILATION -DMONGOC_COMPILATION -DMONGOC_TRACE"
    PHP_LIBMONGOC_CONFIGURE="--with-libbson=bundled --disable-automatic-init-and-cleanup --disable-examples --disable-html-docs --disable--man-pages --disable-silent-rules --quiet"

    if test "$PHP_SYSTEM_CIPHERS" = "yes"; then
      PHP_LIBMONGOC_CONFIGURE="$LIBMONGOC_CONFIG_ARGS --enable-crypto-system-profile"
    fi

    if test "$PHP_DEBUG" = "yes"; then
      PHP_LIBMONGOC_CONFIGURE="$LIBMONGOC_CONFIG_ARGS --enable-debug"
    fi

    m4_include(mp-config-subpackage.m4)
    MP_CONFIG_SUBPACKAGE([src/libmongoc], [$PHP_LIBMONGOC_CONFIGURE CFLAGS="$CFLAGS"])

    PHP_ADD_SOURCES_X(PHP_EXT_DIR(mongodb)[/src/libmongoc/src/libbson/src/bson], $BSON_SOURCES, [$PHP_LIBMONGOC_CFLAGS], shared_objects_mongodb, yes)
    PHP_ADD_SOURCES_X(PHP_EXT_DIR(mongodb)[/src/libmongoc/src/libbson/src/yajl], $YAJL_SOURCES, [$PHP_LIBMONGOC_CFLAGS], shared_objects_mongodb, yes)
    PHP_ADD_SOURCES_X(PHP_EXT_DIR(mongodb)[/src/libmongoc/src/mongoc], $MONGOC_SOURCES, [$PHP_LIBMONGOC_CFLAGS], shared_objects_mongodb, yes)
  fi

  m4_include(src/libmongoc/build/autotools/m4/ax_pthread.m4)
  AX_PTHREAD
  MONGODB_SHARED_LIBADD="$MONGODB_SHARED_LIBADD $PTHREAD_LIBS"

  AC_CHECK_FUNCS([shm_open], [SHM_LIB=], [AC_CHECK_LIB([rt], [shm_open], [SHM_LIB=-lrt], [SHM_LIB=])])
  MONGODB_SHARED_LIBADD="$MONGODB_SHARED_LIBADD $SHM_LIB"

  PHP_SUBST(MONGODB_SHARED_LIBADD)
  PHP_SUBST(EXTRA_LDFLAGS)

  PHP_NEW_EXTENSION(mongodb, $PHP_MONGODB_SOURCES, $ext_shared,, $PHP_MONGODB_CFLAGS)
  PHP_ADD_EXTENSION_DEP(mongodb, date)
  PHP_ADD_EXTENSION_DEP(mongodb, json)
  PHP_ADD_EXTENSION_DEP(mongodb, spl)
  PHP_ADD_EXTENSION_DEP(mongodb, standard)

  PHP_ADD_INCLUDE([$ext_srcdir/src/BSON])
  PHP_ADD_INCLUDE([$ext_srcdir/src/MongoDB])
  PHP_ADD_INCLUDE([$ext_srcdir/src/MongoDB/Exception])
  PHP_ADD_INCLUDE([$ext_srcdir/src/contrib])
  PHP_ADD_BUILD_DIR([$ext_builddir/src/BSON])
  PHP_ADD_BUILD_DIR([$ext_builddir/src/MongoDB])
  PHP_ADD_BUILD_DIR([$ext_builddir/src/MongoDB/Exception])
  PHP_ADD_BUILD_DIR([$ext_builddir/src/contrib])

  if test "$PHP_LIBMONGOC" == "no"; then
    PHP_ADD_INCLUDE([$ext_srcdir/src/libmongoc/src/libbson/src/bson])
    PHP_ADD_INCLUDE([$ext_srcdir/src/libmongoc/src/libbson/src/yajl])
    PHP_ADD_INCLUDE([$ext_srcdir/src/libmongoc/src/mongoc])

    PHP_ADD_BUILD_DIR([$ext_builddir/src/libmongoc/src/libbson/src/bson])
    PHP_ADD_BUILD_DIR([$ext_builddir/src/libmongoc/src/libbson/src/yajl])
    PHP_ADD_BUILD_DIR([$ext_builddir/src/libmongoc/src/mongoc])
  fi

  dnl This must come after PHP_NEW_EXTENSION, otherwise the srcdir won't be set
  PHP_ADD_MAKEFILE_FRAGMENT

  AC_CONFIG_COMMANDS_POST([echo "
mongodb was configured with the following options:

Build configuration:
  CFLAGS                                           : $CFLAGS
  Extra CFLAGS                                     : $STD_CFLAGS $EXTRA_CFLAGS
  Developer  flags (slow)                          : $MAINTAINER_CFLAGS
  Code Coverage flags (extra slow)                 : $COVERAGE_CFLAGS
  System libmongoc                                 : $PHP_LIBMONGOC
  System libbson                                   : $PHP_LIBBSON
  LDFLAGS                                          : $LDFLAGS
  EXTRA_LDFLAGS                                    : $EXTRA_LDFLAGS
  MONGODB_SHARED_LIBADD                            : $MONGODB_SHARED_LIBADD

Please submit bugreports at:
  https://jira.mongodb.org/browse/PHPC

"])
fi

dnl: vim: et sw=2
