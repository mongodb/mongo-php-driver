#! /bin/sh

# Allow invocation from a separate build directory; in that case, we change
# to the source directory to run the auto*, then change back before running configure
srcdir=`dirname "$0"`
test -z "$srcdir" && srcdir=.

ORIGDIR=`pwd`
cd "$srcdir" || exit 1

if [ ! -f README ]; then
    ln -s README.md README
fi

if [ -d .git ]; then
    git submodule init
    git submodule update

    cd src/libmongoc
    NOCONFIGURE=1 ./autogen.sh >/dev/null
    cd ../../

    cd src/libbson
    NOCONFIGURE=1 ./autogen.sh >/dev/null
    cd ../../
fi


cd "$ORIGDIR" || exit 1

