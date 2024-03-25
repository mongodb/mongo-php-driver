#!/usr/bin/env bash
set -o errexit  # Exit the script with error if any of the commands fail

# Find PHP binary path for the requested version
if [ -z "$PHP_PATH" ]; then
  if [ -d "/opt/php/${PHP_VERSION}-64bit/bin" ]; then
     PHP_PATH="/opt/php/${PHP_VERSION}-64bit/bin"
  else
     # Try to find the newest version matching our constant
     PHP_PATH=`find /opt/php/ -maxdepth 1 -type d -name "${PHP_VERSION}*-64bit" -print | sort -V -r | head -n 1`/bin
  fi
fi

if [ ! -x "$PHP_PATH/php" ]; then
   echo "Could not find PHP binaries for version ${PHP_VERSION}. Listing available versions..."
   ls -1 /opt/php
   exit 1
fi

PATH="$PHP_PATH:$PATH"

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
echo "PHP: `php --version | head -n 1`"

# If we're testing a specific version of libmongoc, update submodule sources
if [ -n "$LIBMONGOC_VERSION" ]; then
   php scripts/update-submodule-sources.php
fi

phpize
./configure --enable-mongodb-developer-flags

# configure relies on version information in libmongoc-version-current, but the target is not available until after calling configure
# To work around this, run the make target, then run configure again
if [ -n "$LIBMONGOC_VERSION" ]; then
   make libmongoc-version-current
  ./configure --enable-mongodb-developer-flags
fi

make test TESTS="tests/smoketest.phpt"
