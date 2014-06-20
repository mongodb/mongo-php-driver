#!/bin/bash
sudo apt-get install gdb
phpize
./configure --enable-developer-flags --enable-coverage --quiet
make all install

if [ $? -ne 0 ]; then
	exit 42
fi

echo "extension=phongo" >> `php --ini | grep "Loaded Configuration" | sed -e "s|.*:\s*||"`

MONGO=`which mongo`
mongod --version
ls $MONGO*
pwd
