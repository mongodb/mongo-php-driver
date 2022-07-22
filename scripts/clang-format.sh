#!/bin/sh

CLANG_ARGS="-Werror"

if test x"$1" = xchanged; then
  FILES1=`git ls-files | grep -v "src/contrib" | grep -v "_arginfo.h" | grep '\.[ch]$'`
  FILES2=`git ls-files --others --exclude-standard | grep -v "src/contrib" | grep -v "_arginfo.h" | grep '\.[ch]$'`
  FILES="$FILES1 $FILES2"
else
	FILES1=`git ls-files | grep -v "src/contrib" | grep -v "_arginfo.h" | grep '\.[ch]$'`
	FILES2=`git ls-files --others --exclude-standard | grep -v "src/contrib" | grep -v "_arginfo.h" | grep '\.[ch]$'`
	FILES="$FILES1 $FILES2"
fi

if test x"$1" = xcheck; then
	CLANG_ARGS="$CLANG_ARGS -n"
fi

# Find clang-format, we prefer -6.0, but also allow binaries without -suffix as
# long as they're >= 6.0.0
CLANG_FORMAT=`which clang-format-6.0`

if [ -z "$CLANG_FORMAT" ]; then
	CLANG_FORMAT=`which clang-format`
fi

if [ -z "$CLANG_FORMAT" ]; then
	echo "Couldn't find clang-format"
	exit
fi

VERSION=`$CLANG_FORMAT -version | cut -d " " -f 3`
VERSION_MAJOR=`echo $VERSION | cut -d "." -f 1`

if [ $VERSION_MAJOR -lt 6 ]; then
	echo "Found clang-format $VERSION but we need >= 6.0.0"
	exit
fi

FAILURE=""

# Run formatter
for i in $FILES; do
	$CLANG_FORMAT $CLANG_ARGS -i $i
	[ $? -eq 0 ] || FAILURE="yes"
done

if [ -z "$FAILURE" ]; then
  exit 0
fi

exit 1
