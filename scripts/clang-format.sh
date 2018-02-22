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
# long as they're 6.0.0
CLANG_FORMAT=

which clang-format-6.0
if [ $? = 0 ]; then
	CLANG_FORMAT=`which clang-format-6.0`
fi

which clang-format
if [ $? = 0 ]; then
	VERSION=`clang-format -version | cut -d " " -f 3 | cut -c 1-3`
	if [ x"$VERSION" = "x6.0" ]; then
		CLANG_FORMAT=`which clang-format`
	fi
fi

if [ x"$CLANG_FORMAT" = "x" ]; then
	echo "Couldn't find the right clang-format (needs version 6.0)"
	exit
fi

# Run formatter
for i in $FILES; do
	$CLANG_FORMAT -i $i
done
