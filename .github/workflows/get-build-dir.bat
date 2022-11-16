@ECHO OFF
grep "BUILD_DIR=" Makefile | sed 's/BUILD_DIR=//g' | awk '{$1=$1};1'
