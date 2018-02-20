#!/bin/sh

# by default we select all files
FILES1=`git ls-files | grep -v "src/contrib" | grep '\.[ch]$'`
FILES2=`git ls-files --others --exclude-standard | grep -v "src/contrib" | grep '\.[ch]$'`
FILES="$FILES1 $FILES2"

if test x"$1" = xtest; then
	RETURN=0

	for i in $FILES; do
		clang-format-7 $i > /tmp/formatted.tmp.c
		SAME=`diff -qs $i /tmp/formatted.tmp.c | grep identical`
		if [ "x$SAME" = x ]; then
			echo $i has formatting errors
			RETURN=1
		fi
	done
	
	exit $RETURN
fi

if test x"$1" = xchanged; then
	FILES1=`git diff --name-only | grep -v "src/contrib" | grep '\.[ch]$'`
	FILES2=`git diff --cached --name-only | grep -v "src/contrib" | grep '\.[ch]$'`
	FILES3=`git ls-files --others --exclude-standard | grep '\.[ch]$'`
	FILES="$FILES1 $FILES2 $FILES3"
fi

for i in $FILES; do
	clang-format-7 -i $i
done
