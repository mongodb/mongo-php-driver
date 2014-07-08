dnl config.m4 for extension phongo

PHP_ARG_ENABLE(phongo, whether to enable phongo support,
[  --enable-phongo           Enable phongo support])

PHP_ARG_WITH(libmongoc-dir, for libmongoc,
[  --with-libmongoc-dir[=DIR]   Set the path to libmongoc install prefix.], yes)



if test "$PHONGO" != "no"; then
  AC_MSG_CHECKING(for libmongoc support)

  LIBMONGOC_DIR=""
  dnl Uses the last one found, so stick the PHP_LIBMONGOC_DIR last
  for i in /usr /usr/local $PHP_LIBMONGOC_DIR ; do
    if test -r $i/include/libmongoc-1.0/mongoc.h; then
      LIBMONGOC_DIR=$i
    fi
  done

  if test -r "$LIBMONGOC_DIR"; then
    LIBMONGOC_INCLUDE="-I$PHP_LIBMONGOC_DIR/include/libmongoc-1.0 -I$PHP_LIBMONGOC_DIR/include/libbson-1.0"
    LIBMONGOC_LFLAGS="-L$PHP_LIBMONGOC_DIR/$PHP_LIBDIR"
    LIBMONGOC_LIBS="-lmongoc-1.0 -lbson-1.0"
    AC_MSG_RESULT(found $LIBMONGOC_DIR)
  else
    AC_MSG_RESULT([not found])
    AC_PATH_PROG(PKG_CONFIG, pkg-config, no)

    export ORIG_PKG_CONFIG_PATH="$PKG_CONFIG_PATH"
    if test -x "$PKG_CONFIG"; then
      AC_MSG_CHECKING(for libmongoc using pkg-config)

      export PKG_CONFIG_PATH="$ORIG_PKG_CONFIG_PATH:/usr/local/$PHP_LIBDIR/pkgconfig:/usr/$PHP_LIBDIR/pkgconfig:/opt/$PHP_LIBDIR/pkgconfig:$PHP_LIBMONGOC_DIR"
      if ! $PKG_CONFIG --exists libmongoc-1.0; then
        export PKG_CONFIG_PATH="$ORIG_PKG_CONFIG_PATH"
        AC_MSG_ERROR([Can't find where libmongoc is installed])
      fi

      LIBMONGOC_INCLUDE=`$PKG_CONFIG --cflags-only-I libmongoc-1.0`
      LIBMONGOC_LFLAGS=`$PKG_CONFIG --libs-only-L libmongoc-1.0`
      LIBMONGOC_LIBS=`$PKG_CONFIG --libs-only-l libmongoc-1.0`
      PHP_LIMONGOC_DIR=`$PKG_CONFIG --variable=prefix libmongoc-1.0`
      export PKG_CONFIG_PATH="$ORIG_PKG_CONFIG_PATH"

      AC_MSG_RESULT([found using pkg-config])
    else
      AC_MSG_ERROR([Can't find where libmongoc is installed])
    fi
  fi

  dnl FIXME: we may need to statically link libmongoc....
  dnl AC_CONFIG_SUBDIRS([src/libmongoc])
  dnl sub_configure_args="--enable-debug --enable-debug-symbols=full"
  PHP_ADD_INCLUDE($LIBMONGOC_DIR/include/libmongoc-1.0)
  PHP_ADD_INCLUDE($LIBMONGOC_DIR/include/libbson-1.0)

  PHP_ADD_LIBRARY_WITH_PATH(mongoc-1.0, $LIBMONGOC_DIR/$PHP_LIBDIR, PHONGO_SHARED_LIBADD)
  PHP_SUBST(PHONGO_SHARED_LIBADD)
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


  PHP_SUBST(EXTRA_LDFLAGS)

  PHONGO_BSON="\
      src/bson.c \
  ";
  PHONGO_ROOT="\
      php_phongo.c \
      pongoc.c \
  ";
  PHONGO_INTERFACES="\
      src/Cursor.c \
      src/WriteResult.c \
  ";
  PHONGO_CLASSES="\
      src/Batch.c \
      src/Command.c \
      src/CommandCursor.c \
      src/CommandResult.c \
      src/CursorId.c \
      src/Result.c \
      src/GeneratedId.c \
      src/Manager.c \
      src/Query.c \
      src/QueryCursor.c \
      src/ReadPreference.c \
      src/Server.c \
      src/WriteConcernError.c \
      src/WriteError.c \
  ";

  if test "$ext_shared" = "no"; then
    PHP_ADD_SOURCES(PHP_EXT_DIR(phongo), $PHONGO_BSON)
    PHP_ADD_SOURCES(PHP_EXT_DIR(phongo), $PHONGO_INTERFACES)
    PHP_ADD_SOURCES(PHP_EXT_DIR(phongo), $PHONGO_CLASSES)
  else
    PHP_ADD_SOURCES_X(PHP_EXT_DIR(phongo), $PHONGO_BSON,               $EXTRA_CFLAGS, shared_objects_phongo, yes)
    PHP_ADD_SOURCES_X(PHP_EXT_DIR(phongo), $PHONGO_INTERFACES,         $EXTRA_CFLAGS, shared_objects_phongo, yes)
    PHP_ADD_SOURCES_X(PHP_EXT_DIR(phongo), $PHONGO_CLASSES,            $EXTRA_CFLAGS, shared_objects_phongo, yes)
  fi
  PHP_NEW_EXTENSION(phongo,    $PHONGO_ROOT, $ext_shared,, $EXTRA_CFLAGS)
  PHP_ADD_EXTENSION_DEP(phongo, spl)


  dnl This must come after PHP_NEW_EXTENSION, otherwise the srcdir won't be set
  if test "$PHP_COVERAGE" = "yes"; then
      PHP_ADD_MAKEFILE_FRAGMENT
  fi

fi

dnl: vim: et sw=2
