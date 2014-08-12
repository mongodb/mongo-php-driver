dnl config.m4 for extension phongo
syscmd("./autogen.sh")

PHP_ARG_ENABLE(phongo, whether to enable phongo support,
[  --enable-phongo           Enable phongo support])



MONGOC_SYMBOL_SUFFIX="priv"

if test "$PHONGO" != "no"; then
  AC_MSG_CHECKING(configuring libmongoc)
  AC_MSG_RESULT(...)

  dnl Run libmongoc and libbson configure scripts..
  AC_CONFIG_SUBDIRS([src/libmongoc])
  AC_CONFIG_SUBDIRS([src/libbson])
  PHP_ADD_INCLUDE(src/libmongoc/src/mongoc/)
  PHP_ADD_INCLUDE(src/libbson/src/bson/)
  dnl ...with hardcoded arguments
  ac_configure_args="--enable-debug --enable-tracing --enable-debug-symbols=full --disable-hardening --enable-examples=no --enable-man-pages=no --enable-sasl=no --enable-tests=no --enable-ssl=no --enable-silent-rules --quiet"
  _AC_OUTPUT_SUBDIRS
  no_recursion=yes
  PHP_ADD_LIBRARY_WITH_PATH(mongoc-priv, src/libmongoc/.libs, PHONGO_SHARED_LIBADD)
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

  PHONGO_SHARED_DEPENDENCIES="phongodep"
  PHP_SUBST(PHONGO_SHARED_DEPENDENCIES)

  PHP_NEW_EXTENSION(phongo,    $PHONGO_ROOT, $ext_shared,, $EXTRA_CFLAGS)
  PHP_ADD_EXTENSION_DEP(phongo, spl)


  dnl This must come after PHP_NEW_EXTENSION, otherwise the srcdir won't be set
  PHP_ADD_MAKEFILE_FRAGMENT

AC_CONFIG_COMMANDS_POST([echo "
phongo was configured with the following options:

Build configuration:
  Enable developers flags (slow)                   : $_EXTRA_CFLAGS
  CFLAGS                                           : $CFLAGS

Submit bugreports at:
  https://jira.mongodb.org/browse/PHP

"])
fi

dnl: vim: et sw=2
