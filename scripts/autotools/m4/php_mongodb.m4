dnl
dnl PHP_MONGODB_ADD_SOURCES(source-path, sources [, special-flags])
dnl
dnl Adds sources which are located relative to source-path. source-path should
dnl be relative to the extension directory (i.e. PHP_EXT_DIR). special-flags
dnl will be passed to the compiler.
dnl
dnl This macro will call PHP_ADD_SOURCES or PHP_ADD_SOURCES_X depending on
dnl whether the extension is being built statically or as a shared module.
dnl
AC_DEFUN([PHP_MONGODB_ADD_SOURCES],[
  _src_path=PHP_EXT_DIR(mongodb)

  dnl Join extension directory and source path
  case $_src_path in
  ""[)] _src_path="$1" ;;
  */[)] _src_path="$_src_path$1" ;;
  *[)]  _src_path="$_src_path/$1" ;;
  esac

  dnl Trim trailing slash from source path
  case $_src_path in
  */[)] _src_path=${_src_path%?}
  esac

  if test "$ext_shared" = "no"; then
    PHP_ADD_SOURCES($_src_path, [$2], [$3])
  else
    PHP_ADD_SOURCES_X($_src_path, [$2], [$3], shared_objects_mongodb, yes)
  fi
])

dnl
dnl PHP_MONGODB_ADD_INCLUDE(path)
dnl
dnl Adds an include path relative to the extension source directory (i.e.
dnl PHP_EXT_SRCDIR).
dnl
AC_DEFUN([PHP_MONGODB_ADD_INCLUDE],[
  PHP_ADD_INCLUDE(PHP_EXT_SRCDIR(mongodb)[/][$1])
])

dnl
dnl PHP_MONGODB_ADD_BUILD_DIR(path)
dnl
dnl Adds a build directory relative to the extension build directory (i.e.
dnl PHP_EXT_BUILDDIR).
dnl
AC_DEFUN([PHP_MONGODB_ADD_BUILD_DIR],[
  PHP_ADD_BUILD_DIR(PHP_EXT_BUILDDIR(mongodb)[/][$1])
])
