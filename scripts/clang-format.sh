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

for i in $FILES; do
	clang-format-7 -i $i
done
