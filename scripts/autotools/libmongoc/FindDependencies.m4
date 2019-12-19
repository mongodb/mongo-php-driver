# Solaris needs to link against socket libs
if test "$os_solaris" = "yes"; then
    PHP_MONGODB_BUNDLED_CFLAGS="$PHP_MONGODB_BUNDLED_CFLAGS -D__EXTENSIONS__"
    PHP_MONGODB_BUNDLED_CFLAGS="$PHP_MONGODB_BUNDLED_CFLAGS -D_XOPEN_SOURCE=1"
    PHP_MONGODB_BUNDLED_CFLAGS="$PHP_MONGODB_BUNDLED_CFLAGS -D_XOPEN_SOURCE_EXTENDED=1"
    PHP_ADD_LIBRARY([socket],,[MONGODB_SHARED_LIBADD])
    PHP_ADD_LIBRARY([nsl],,[MONGODB_SHARED_LIBADD])
fi

# Check for shm functions
AC_CHECK_FUNCS([shm_open],[],[
  AC_CHECK_LIB([rt],
               [shm_open],
               [PHP_ADD_LIBRARY([rt],,[MONGODB_SHARED_LIBADD])],
               [])
])

# Check for sched_getcpu
AC_CHECK_FUNC([sched_getcpu],
              [AC_SUBST(MONGOC_HAVE_SCHED_GETCPU, 1)],
              [AC_SUBST(MONGOC_HAVE_SCHED_GETCPU, 0)])

AC_CHECK_TYPE([socklen_t],
              [AC_SUBST(MONGOC_HAVE_SOCKLEN, 1)],
              [AC_SUBST(MONGOC_HAVE_SOCKLEN, 0)],
              [#include <sys/socket.h>])

AC_CHECK_MEMBER([struct sockaddr_storage.ss_family],
                [AC_SUBST(MONGOC_HAVE_SS_FAMILY, 0)],
                [AC_SUBST(MONGOC_HAVE_SS_FAMILY, 1)],
                [#include <sys/socket.h>])

# Check for pthreads. libmongoc's original FindDependencies.m4 script did not
# require pthreads, but it does appear to be necessary on non-Windows platforms
# based on mongoc-openssl.c and mongoc-thread-private.h.
AX_PTHREAD([
  PHP_MONGODB_BUNDLED_CFLAGS="$PHP_MONGODB_BUNDLED_CFLAGS $PTHREAD_CFLAGS"
  PHP_EVAL_LIBLINE([$PTHREAD_LIBS],[MONGODB_SHARED_LIBADD])

  # PTHREAD_CFLAGS may come back as "-pthread", which should also be used when
  # linking. We can trust PHP_EVAL_LIBLINE to ignore other values.
  PHP_EVAL_LIBLINE([$PTHREAD_CFLAGS],[MONGODB_SHARED_LIBADD])
],[
  AC_MSG_ERROR([libmongoc requires pthreads on non-Windows platforms.])
])
