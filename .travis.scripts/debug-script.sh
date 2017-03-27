#!/bin/sh

EXP=`echo $1 | sed 's/diff/exp/'`
OUT=`echo $1 | sed 's/diff/out/'`

echo "FILE $1"

echo $EXP
echo "======"
cat $EXP
echo "======"

echo $OUT
echo "======"
cat $OUT
echo "======"

echo

