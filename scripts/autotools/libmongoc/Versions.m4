MONGOC_CURRENT_FILE=[]PHP_EXT_SRCDIR(mongodb)[/src/LIBMONGOC_VERSION_CURRENT]
libmongoc_VERSION_FULL=$(cat $MONGOC_CURRENT_FILE)

dnl Ensure newline for "cut" implementations that need it, e.g. HP-UX.
libmongoc_VERSION_MAJOR=$( (cat $MONGOC_CURRENT_FILE; echo) | cut -d- -f1 | cut -d. -f1 )
libmongoc_VERSION_MINOR=$( (cat $MONGOC_CURRENT_FILE; echo) | cut -d- -f1 | cut -d. -f2 )
libmongoc_VERSION_PATCH=$( (cat $MONGOC_CURRENT_FILE; echo) | cut -d- -f1 | cut -d. -f3 )
libmongoc_VERSION_PRERELEASE=$(cut -s -d- -f2 $MONGOC_CURRENT_FILE)

AC_SUBST(libmongoc_VERSION_FULL)
AC_SUBST(libmongoc_VERSION_MAJOR)
AC_SUBST(libmongoc_VERSION_MINOR)
AC_SUBST(libmongoc_VERSION_PATCH)
AC_SUBST(libmongoc_VERSION_PRERELEASE)
