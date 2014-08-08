dnl config.m4 for extension phongo

PHP_ARG_ENABLE(phongo, whether to enable phongo support,
[  --enable-phongo           Enable phongo support])

PHP_ARG_WITH(libmongoc-dir, for libmongoc,
[  --with-libmongoc-dir[=DIR]   Set the path to libmongoc install prefix.], yes)



MONGOC_SYMBOL_SUFFIX="priv"

if test "$PHONGO" != "no"; then
  AC_MSG_CHECKING(for libmongoc support)

  MONGOC_DIR=""
  dnl Uses the last one found, so stick the PHP_MONGOC_DIR last
  for i in /usr /usr/local $PHP_MONGOC_DIR ; do
    if test -r $i/include/libmongoc-$MONGOC_SYMBOL_SUFFIX/mongoc.h; then
      MONGOC_DIR=$i
    fi
  done

  if test -r "$MONGOC_DIR"; then
    MONGOC_INCLUDE="-I$PHP_MONGOC_DIR/include/libmongoc-$MONGOC_SYMBOL_SUFFIX -I$PHP_MONGOC_DIR/include/libbson-1.0"
    MONGOC_CFLAGS="-DMONGOC_I_AM_A_DRIVER"
    MONGOC_LIBS="-lmongoc-$MONGOC_SYMBOL_SUFFIX -lbson-1.0"
    AC_MSG_RESULT(found $MONGOC_DIR)
  else
    AC_MSG_RESULT([not found])
    AC_PATH_PROG(PKG_CONFIG, pkg-config, no)

    export ORIG_PKG_CONFIG_PATH="$PKG_CONFIG_PATH"
    if test -x "$PKG_CONFIG"; then
      AC_MSG_CHECKING(for libmongoc using pkg-config)

      export PKG_CONFIG_PATH="$ORIG_PKG_CONFIG_PATH:/usr/local/$PHP_LIBDIR/pkgconfig:/usr/$PHP_LIBDIR/pkgconfig:/opt/$PHP_LIBDIR/pkgconfig:$PHP_MONGOC_DIR"
      if ! $PKG_CONFIG --exists libmongoc-$MONGOC_SYMBOL_SUFFIX; then
        if $PKG_CONFIG --exists libmongoc-1.0; then
          export PKG_CONFIG_PATH="$ORIG_PKG_CONFIG_PATH"
          AC_MSG_ERROR([missing libmongoc symbols, please reinstall mongo-c-driver -priv object])
        fi
        export PKG_CONFIG_PATH="$ORIG_PKG_CONFIG_PATH"
        AC_MSG_ERROR([Can't find where libmongoc is installed])
      fi

      MONGOC_INCLUDE=`$PKG_CONFIG --cflags-only-I libmongoc-$MONGOC_SYMBOL_SUFFIX`
      MONGOC_CFLAGS=`$PKG_CONFIG --cflags-only-other libmongoc-$MONGOC_SYMBOL_SUFFIX`
      MONGOC_LIBS=`$PKG_CONFIG --libs libmongoc-$MONGOC_SYMBOL_SUFFIX`
      MONGOC_DIR=`$PKG_CONFIG --variable=prefix libmongoc-$MONGOC_SYMBOL_SUFFIX`
      export PKG_CONFIG_PATH="$ORIG_PKG_CONFIG_PATH"

      AC_MSG_RESULT([found using pkg-config])
    else
      export PKG_CONFIG_PATH="$ORIG_PKG_CONFIG_PATH"
      AC_MSG_ERROR([pkg-config not available and no path provided])
    fi
  fi

  dnl FIXME: we may need to statically link libmongoc....
  dnl AC_CONFIG_SUBDIRS([src/libmongoc])
  dnl sub_configure_args="--enable-debug --enable-debug-symbols=full"
  dnl PHP_ADD_INCLUDE($MONGOC_DIR/include/libmongoc-$MONGOC_SYMBOL_SUFFIX)
  dnl PHP_ADD_LIBRARY_WITH_PATH(mongoc-$MONGOC_SYMBOL_SUFFIX, $MONGOC_DIR/$PHP_LIBDIR, PHONGO_SHARED_LIBADD)
  PHP_EVAL_LIBLINE($MONGOC_LIBS, PHONGO_SHARED_LIBADD)
  PHP_SUBST(PHONGO_SHARED_LIBADD)
  PHP_EVAL_INCLINE($MONGOC_INCLUDE)


  AC_DEFINE(HAVE_MONGOC, 1, [Kinda useless extension without it..])


  PHP_ARG_ENABLE(developer-flags, whether to enable developer build flags,
  [  --enable-developer-flags   Enable developer flags],, no)

  if test "$PHP_DEVELOPER_FLAGS" = "yes"; then
    dnl Warn about functions which might be candidates for format attributes
    PHP_CHECK_GCC_ARG(-Wmissing-format-attribute,       _EXTRA_CFLAGS="$_EXTRA_CFLAGS -Wmissing-format-attribute")
    dnl Avoid duplicating values for an enum
    PHP_CHECK_GCC_ARG(-Wduplicate-enum,                 _EXTRA_CFLAGS="$_EXTRA_CFLAGS -Wduplicate-enum")
    dnl Warns on mismatches between #ifndef and #define header guards
    PHP_CHECK_GCC_ARG(-Wheader-guard,                   _EXTRA_CFLAGS="$_EXTRA_CFLAGS -Wheader-guard")
    dnl logical not of a non-boolean expression
    PHP_CHECK_GCC_ARG(-Wlogical-not-parentheses,        _EXTRA_CFLAGS="$_EXTRA_CFLAGS -Wlogical-not-parentheses")
    dnl Warn about suspicious uses of logical operators in expressions
    PHP_CHECK_GCC_ARG(-Wlogical-op,                     _EXTRA_CFLAGS="$_EXTRA_CFLAGS -Wlogical-op")
    dnl memory error detector.
    dnl FIXME: -fsanitize=address,undefined for clang. The PHP_CHECK_GCC_ARG macro isn't happy about that string :(
    PHP_CHECK_GCC_ARG(-fsanitize-address,               _EXTRA_CFLAGS="$_EXTRA_CFLAGS -fsanitize-address")
    dnl Enable frame debugging
    PHP_CHECK_GCC_ARG(-fno-omit-frame-pointer,          _EXTRA_CFLAGS="$_EXTRA_CFLAGS -fno-omit-frame-pointer")
    dnl Make sure we don't optimize calls
    PHP_CHECK_GCC_ARG(-fno-optimize-sibling-calls,      _EXTRA_CFLAGS="$_EXTRA_CFLAGS -fno-optimize-sibling-calls")
    PHP_CHECK_GCC_ARG(-Wlogical-op-parentheses,         _EXTRA_CFLAGS="$_EXTRA_CFLAGS -Wlogical-op-parentheses")
    PHP_CHECK_GCC_ARG(-Wbool-conversion,                _EXTRA_CFLAGS="$_EXTRA_CFLAGS -Wbool-conversion")
    PHP_CHECK_GCC_ARG(-Wloop-analysis,                  _EXTRA_CFLAGS="$_EXTRA_CFLAGS -Wloop-analysis")
    PHP_CHECK_GCC_ARG(-Wsizeof-array-argument,          _EXTRA_CFLAGS="$_EXTRA_CFLAGS -Wsizeof-array-argument")
    PHP_CHECK_GCC_ARG(-Wstring-conversion,              _EXTRA_CFLAGS="$_EXTRA_CFLAGS -Wstring-conversion")
    PHP_CHECK_GCC_ARG(-Wno-variadic-macros,             _EXTRA_CFLAGS="$_EXTRA_CFLAGS -Wno-variadic-macros")
    PHP_CHECK_GCC_ARG(-Wno-sign-compare,                _EXTRA_CFLAGS="$_EXTRA_CFLAGS -Wno-sign-compare")
    PHP_CHECK_GCC_ARG(-fstack-protector,                _EXTRA_CFLAGS="$_EXTRA_CFLAGS -fstack-protector")
    PHP_CHECK_GCC_ARG(-fno-exceptions,                  _EXTRA_CFLAGS="$_EXTRA_CFLAGS -fno-exceptions")
    PHP_CHECK_GCC_ARG(-Wformat-security,                _EXTRA_CFLAGS="$_EXTRA_CFLAGS -Wformat-security")
    PHP_CHECK_GCC_ARG(-Wformat-nonliteral,              _EXTRA_CFLAGS="$_EXTRA_CFLAGS -Wformat-nonliteral")
    PHP_CHECK_GCC_ARG(-Winit-self,                      _EXTRA_CFLAGS="$_EXTRA_CFLAGS -Winit-self")
    PHP_CHECK_GCC_ARG(-Wwrite-strings,                  _EXTRA_CFLAGS="$_EXTRA_CFLAGS -Wwrite-strings")
    PHP_CHECK_GCC_ARG(-Wenum-compare,                   _EXTRA_CFLAGS="$_EXTRA_CFLAGS -Wenum-compare")
    PHP_CHECK_GCC_ARG(-Wempty-body,                     _EXTRA_CFLAGS="$_EXTRA_CFLAGS -Wempty-body")
    PHP_CHECK_GCC_ARG(-Wparentheses,                    _EXTRA_CFLAGS="$_EXTRA_CFLAGS -Wparentheses")


    EXTRA_CFLAGS="-g -O0 -Wall -Wextra $_EXTRA_CFLAGS "
    dnl EXTRA_LDFLAGS="-Wl,--no-undefined"
  fi


  PHP_ARG_ENABLE(coverage, whether to enable code coverage,
  [  --enable-coverage Enable developer code coverage information],, no)

  if test "$PHP_COVERAGE" = "yes"; then
      PHP_CHECK_GCC_ARG(-fprofile-arcs,                     _EXTRA_FLAGS="$_EXTRA_FLAGS -fprofile-arcs")
      PHP_CHECK_GCC_ARG(-ftest-coverage,                    _EXTRA_FLAGS="$_EXTRA_FLAGS -ftest-coverage")
      EXTRA_CFLAGS="$EXTRA_CFLAGS $_EXTRA_FLAGS "
      EXTRA_LDFLAGS="$_EXTRA_FLAGS"
  fi

  PHONGO_BSON="\
      src/bson.c \
  ";
  PHONGO_BSON_CLASSES="\
    src/BSON/Type.c \
    src/BSON/Binary.c \
    src/BSON/DBRef.c \
    src/BSON/Int32.c \
    src/BSON/Int64.c \
    src/BSON/Javascript.c \
    src/BSON/Log.c \
    src/BSON/MaxKey.c \
    src/BSON/MinKey.c \
    src/BSON/ObjectID.c \
    src/BSON/Pool.c \
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
      src/MongoDB/GeneratedId.c \
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

  EXTRA_CFLAGS="$EXTRA_CFLAGS $MONGOC_CFLAGS"

  if test "$ext_shared" = "no"; then
    PHP_ADD_SOURCES(PHP_EXT_DIR(phongo), $PHONGO_BSON)
    PHP_ADD_SOURCES(PHP_EXT_DIR(phongo), $PHONGO_BSON_CLASSES)
    PHP_ADD_SOURCES(PHP_EXT_DIR(phongo), $PHONGO_MONGODB_CLASSES)
  else
    PHP_ADD_SOURCES_X(PHP_EXT_DIR(phongo), $PHONGO_BSON,               $EXTRA_CFLAGS, shared_objects_phongo, yes)
    PHP_ADD_SOURCES_X(PHP_EXT_DIR(phongo), $PHONGO_BSON_CLASSES,       $EXTRA_CFLAGS, shared_objects_phongo, yes)
    PHP_ADD_SOURCES_X(PHP_EXT_DIR(phongo), $PHONGO_MONGODB_CLASSES,    $EXTRA_CFLAGS, shared_objects_phongo, yes)
  fi
  PHP_NEW_EXTENSION(phongo,    $PHONGO_ROOT, $ext_shared,, $EXTRA_CFLAGS)
  PHP_ADD_EXTENSION_DEP(phongo, spl)


  dnl This must come after PHP_NEW_EXTENSION, otherwise the srcdir won't be set
  PHP_ADD_MAKEFILE_FRAGMENT

fi

dnl: vim: et sw=2
