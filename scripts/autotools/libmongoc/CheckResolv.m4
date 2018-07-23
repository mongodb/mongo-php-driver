dnl Disable Windows DNSAPI
AC_SUBST(MONGOC_HAVE_DNSAPI, 0)

found_resolv="no"

old_LIBS="$LIBS"
LIBS="$LIBS -lresolv"

dnl Thread-safe DNS query function for _mongoc_client_get_srv.
dnl Could be a macro, not a function, so check with AC_LINK_IFELSE.
AC_MSG_CHECKING([for res_nsearch])
AC_LINK_IFELSE([AC_LANG_PROGRAM([[
   #include <sys/types.h>
   #include <netinet/in.h>
   #include <arpa/nameser.h>
   #include <resolv.h>
]], [[
   int len;
   unsigned char reply[1024];
   res_state statep;
   len = res_nsearch(
      statep, "example.com", ns_c_in, ns_t_srv, reply, sizeof(reply));
]])], [
   AC_MSG_RESULT([yes])
   AC_SUBST(MONGOC_HAVE_RES_SEARCH, 0)
   AC_SUBST(MONGOC_HAVE_RES_NSEARCH, 1)
   found_resolv="yes"

   dnl We have res_nsearch. Call res_ndestroy (BSD/Mac) or res_nclose (Linux)?
   AC_MSG_CHECKING([for res_ndestroy])
   AC_LINK_IFELSE([AC_LANG_PROGRAM([[
      #include <sys/types.h>
      #include <netinet/in.h>
      #include <arpa/nameser.h>
      #include <resolv.h>
   ]], [[
      res_state statep;
      res_ndestroy(statep);
   ]])], [
      AC_MSG_RESULT([yes])
      AC_SUBST(MONGOC_HAVE_RES_NDESTROY, 1)
      AC_SUBST(MONGOC_HAVE_RES_NCLOSE, 0)
   ], [
      AC_MSG_RESULT([no])
      AC_SUBST(MONGOC_HAVE_RES_NDESTROY, 0)
      AC_MSG_CHECKING([for res_nclose])
      AC_LINK_IFELSE([AC_LANG_PROGRAM([[
         #include <sys/types.h>
         #include <netinet/in.h>
         #include <arpa/nameser.h>
         #include <resolv.h>
      ]], [[
         res_state statep;
         res_nclose(statep);
      ]])], [
         AC_MSG_RESULT([yes])
         AC_SUBST(MONGOC_HAVE_RES_NCLOSE, 1)
      ], [
         AC_MSG_RESULT([no])
         AC_SUBST(MONGOC_HAVE_RES_NCLOSE, 0)
      ])
   ])
],[
   AC_MSG_RESULT([no])
   AC_SUBST(MONGOC_HAVE_RES_NSEARCH, 0)
   AC_SUBST(MONGOC_HAVE_RES_NDESTROY, 0)
   AC_SUBST(MONGOC_HAVE_RES_NCLOSE, 0)

   dnl Thread-unsafe function.
   AC_MSG_CHECKING([for res_search])
   AC_LINK_IFELSE([AC_LANG_PROGRAM([[
      #include <sys/types.h>
      #include <netinet/in.h>
      #include <arpa/nameser.h>
      #include <resolv.h>
   ]], [[
      int len;
      unsigned char reply[1024];
      len = res_search("example.com", ns_c_in, ns_t_srv, reply, sizeof(reply));
   ]])], [
      AC_MSG_RESULT([yes])
      AC_SUBST(MONGOC_HAVE_RES_SEARCH, 1)
      found_resolv="yes"
   ], [
      AC_MSG_RESULT([no])
      AC_SUBST(MONGOC_HAVE_RES_SEARCH, 0)
   ])
])

LIBS="$old_LIBS"

AS_IF([test "$found_resolv" = "yes"],[
  PHP_ADD_LIBRARY([resolv],,[MONGODB_SHARED_LIBADD])
])
