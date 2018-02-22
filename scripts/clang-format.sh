#!/bin/sh

if test x"$1" = x; then 
	FILES1=`git ls-files | grep -v "src/contrib" | grep '\.[ch]$'`
	FILES2=`git ls-files --others --exclude-standard | grep -v "src/contrib" | grep '\.[ch]$'`
	FILES="$FILES1 $FILES2"
fi
if test x"$1" = xchanged; then 
	FILES1=`git diff --name-only | grep -v "src/contrib" | grep '\.[ch]$'`
	FILES2=`git diff --cached --name-only | grep -v "src/contrib" | grep '\.[ch]$'`
	FILES3=`git ls-files --others --exclude-standard | grep '\.[ch]$'`
	FILES="$FILES1 $FILES2 $FILES3"
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

# Run formatter
for i in $FILES; do
	$CLANG_FORMAT -i $i
done
