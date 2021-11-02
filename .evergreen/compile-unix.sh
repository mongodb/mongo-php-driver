#!/bin/sh
set -o errexit  # Exit the script with error if any of the commands fail

# Supported/used environment variables:
#   MARCH             Machine Architecture. Defaults to lowercase uname -m
#   LIBMONGOC_VERSION Optional libmongoc version (regenerate version file if set)

# Automatically retrieve the machine architecture, lowercase, unless provided
# as an environment variable (e.g. to force 32bit)
[ -z "$MARCH" ] && MARCH=$(uname -m | tr '[:upper:]' '[:lower:]')
echo "MARCH: $MARCH"

# Any architecture specific configuration here
case "$MARCH" in
   i386)
      CFLAGS="$CFLAGS -m32 -march=i386"
   ;;
   x86_64)
      CFLAGS="$CFLAGS -m64 -march=x86-64"
   ;;
   ppc64le)
      CFLAGS="$CFLAGS -mcpu=power8 -mtune=power8 -mcmodel=medium"
   ;;
esac

# Get the kernel name, lowercased
OS=$(uname -s | tr '[:upper:]' '[:lower:]')
echo "OS: $OS"

# Operating system specific tweaks
case "$OS" in
   darwin)
   ;;

   linux)
      # Make linux builds a tad faster by parallelise the build
      cpus=$(grep -c '^processor' /proc/cpuinfo)
      MAKEFLAGS="-j${cpus}"
   ;;

   sunos)
      # Most normal build tools on the Solaris servers lives here
      PATH="/opt/mongodbtoolchain/bin:$PATH"
   ;;
esac

# Report the current PHP version
echo "PHP: `php --version | head -1`"

phpize
./configure --enable-mongodb-developer-flags

# If we're testing a specific version of libmongoc, regenerate the version file
if [ -n "$LIBMONGOC_VERSION" ]; then
   make libmongoc-version-current
fi

make
