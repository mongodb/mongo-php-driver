BSON_CURRENT_FILE=[]PHP_EXT_SRCDIR(mongodb)[/src/LIBMONGOC_VERSION_CURRENT]
libbson_VERSION_FULL=$(cat $BSON_CURRENT_FILE)

dnl Ensure newline for "cut" implementations that need it, e.g. HP-UX.
libbson_VERSION_MAJOR=$( (cat $BSON_CURRENT_FILE; echo) | cut -d- -f1 | cut -d. -f1 )
libbson_VERSION_MINOR=$( (cat $BSON_CURRENT_FILE; echo) | cut -d- -f1 | cut -d. -f2 )
libbson_VERSION_PATCH=$( (cat $BSON_CURRENT_FILE; echo) | cut -d- -f1 | cut -d. -f3 )
libbson_VERSION_PRERELEASE=$(cut -s -d- -f2 $BSON_CURRENT_FILE)

AC_SUBST(libbson_VERSION_FULL)
AC_SUBST(libbson_VERSION_MAJOR)
AC_SUBST(libbson_VERSION_MINOR)
AC_SUBST(libbson_VERSION_PATCH)
AC_SUBST(libbson_VERSION_PRERELEASE)
