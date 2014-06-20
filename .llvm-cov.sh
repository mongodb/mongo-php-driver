#!/bin/sh
#

# Stupid stuff
# first executed with -v to get the version number - which xcodes gcov doesn't support
# then executed normally
if [ "$1" = "-v" ]; then
    echo "llvm-cov-wrapper 4.2.1"
    exit 0
else
    gcov $*
fi

