#!/bin/sh

EXP=`echo $1 | sed 's/.diff$/.exp/'`
OUT=`echo $1 | sed 's/.diff$/.out/'`
SEP="--------------------------------------------------------------------------------"

echo $EXP
echo $SEP
cat $EXP; echo
echo $SEP; echo

echo $OUT
echo $SEP
cat $OUT; echo
echo $SEP; echo
