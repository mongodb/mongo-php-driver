#!/bin/bash

sudo apt-get install gdb
wget http://downloads.sourceforge.net/ltp/lcov-1.11.tar.gz
mkdir tmp-lcov
tar zxf lcov* -C tmp-lcov --strip-components=1
(cd tmp-lcov && sudo make install)
rm -rf tmp-lcov

pushd src
wget https://github.com/mongodb/mongo-c-driver/releases/download/0.96.4/mongo-c-driver-0.96.4.tar.gz
mkdir libmongoc
tar zxf mongo-c-driver* -C libmongoc --strip-components=1
(cd libmongoc && ./configure --quiet && make all >/dev/null && sudo make install)
popd


phpize
./configure --enable-coverage --quiet
make all && sudo make install
rm -rf src/libmongoc # coveralls may pick it up and lie about our coverage

if [ $? -ne 0 ]; then
	exit 42
fi

echo "extension=phongo" >> `php --ini | grep "Loaded Configuration" | sed -e "s|.*:\s*||"`

MONGO=`which mongo`
mongod --version
ls $MONGO*
pwd


