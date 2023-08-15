dnl Enable POSIX features up to POSIX.1-2008 plus the XSI extension and BSD-derived definitions.
dnl Both _BSD_SOURCE and _DEFAULT_SOURCE are defined for backwards-compatibility with glibc 2.19 and earlier.
dnl _BSD_SOURCE and _DEFAULT_SOURCE are required by `getpagesize`, `h_errno`, etc.
dnl _XOPEN_SOURCE=700 is required by `strnlen`, `strerror_l`, etc.
dnl https://man7.org/linux/man-pages/man7/feature_test_macros.7.html
dnl https://pubs.opengroup.org/onlinepubs/7908799/xsh/compilation.html
CPPFLAGS="$CPPFLAGS -D_XOPEN_SOURCE=700 -D_BSD_SOURCE -D_DEFAULT_SOURCE"
PHP_MONGODB_BUNDLED_CFLAGS="$PHP_MONGODB_BUNDLED_CFLAGS -D_XOPEN_SOURCE=700 -D_BSD_SOURCE -D_DEFAULT_SOURCE"

dnl Enable non-standard features on FreeBSD with __BSD_VISIBLE=1
if test "$os_freebsd" = "yes"; then
    CPPFLAGS="$CPPFLAGS -D__BSD_VISIBLE=1"
    PHP_MONGODB_BUNDLED_CFLAGS="$PHP_MONGODB_BUNDLED_CFLAGS -D__BSD_VISIBLE=1"
fi

AS_IF([test "$os_darwin" = "yes"],[
    dnl Non-POSIX extensions are required by `_SC_NPROCESSORS_ONLN`.
    dnl https://opensource.apple.com/source/Libc/Libc-1439.40.11/gen/compat.5.auto.html
    CPPFLAGS="$CPPFLAGS -D_DARWIN_C_SOURCE"
    PHP_MONGODB_BUNDLED_CFLAGS="$PHP_MONGODB_BUNDLED_CFLAGS -D_DARWIN_C_SOURCE"

    dnl Ignore OpenSSL deprecation warnings on OSX
    AX_CHECK_COMPILE_FLAG([-Wno-deprecated-declarations], [STD_CFLAGS="$STD_CFLAGS -Wno-deprecated-declarations"])

    dnl We know there are some cast-align issues on OSX
    AX_CHECK_COMPILE_FLAG([-Wno-cast-align], [STD_CFLAGS="$STD_CFLAGS -Wno-cast-align"])
    AX_CHECK_COMPILE_FLAG([-Wno-unneeded-internal-declaration], [STD_CFLAGS="$STD_CFLAGS -Wno-unneeded-internal-declaration"])
    AX_CHECK_COMPILE_FLAG([-Wno-error=unused-command-line-argument], [STD_CFLAGS="$STD_CFLAGS -Wno-error=unused-command-line-argument"])
])
