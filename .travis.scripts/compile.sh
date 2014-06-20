#!/bin/bash
sudo apt-get install gdb 

wget https://github.com/mongodb/mongo-c-driver/releases/download/0.96.2/mongo-c-driver-0.96.2.tar.gz
tar zxvf mongo-c-driver*
(cd mongo-c-driver* && ./configure && make all && sudo make install)


phpize
./configure --enable-coverage --quiet
make all && sudo make install

if [ $? -ne 0 ]; then
	exit 42
fi

echo "extension=phongo" >> `php --ini | grep "Loaded Configuration" | sed -e "s|.*:\s*||"`

MONGO=`which mongo`
mongod --version
ls $MONGO*
pwd


