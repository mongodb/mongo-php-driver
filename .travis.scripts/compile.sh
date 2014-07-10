#!/bin/bash

MONGOC_VERSION=master
LIBBSON_VERSION=master
LCOV_VERSION=1.11


abort_on_failure () {
	if [ $1 -ne 0 ]; then
		echo "Last command failed, exising"
		exit 42;
	fi
}


sudo apt-get install gdb
mkdir tmp-lcov
pushd tmp-lcov
	wget -O lcov.tar.gz http://downloads.sourceforge.net/ltp/lcov-${LCOV_VERSION}.tar.gz
	tar zxf lcov.tar.gz --strip-components=1
	sudo make install
popd

pushd src
	if [ "$MONGOC_VERSION" = "master" ]; then
		wget -O libmongoc.tar.gz https://github.com/mongodb/mongo-c-driver/archive/master.tar.gz
		abort_on_failure $?
	else
		wget -O libmongoc.tar.gz https://github.com/mongodb/mongo-c-driver/releases/download/${MONGOC_VERSION}/mongo-c-driver-${MONGOC_VERSION}.tar.gz
		abort_on_failure $?
	fi


	mkdir libmongoc
	pushd libmongoc
		tar zxf ../libmongoc.tar.gz --strip-components=1
		abort_on_failure $?

		pushd src
			if [ "$LIBBSON_VERSION" = "master" ]; then
				wget -O libbson.tar.gz https://github.com/mongodb/libbson/archive/master.tar.gz
				abort_on_failure $?
			else
				wget -O libbson.tar.gz https://github.com/mongodb/libbson/releases/download/${LIBBSON_VERSION}/libbson-${LIBBSON_VERSION}.tar.gz
				abort_on_failure $?
			fi

			pushd libbson
				tar zxf ../libbson.tar.gz --strip-components=1

				./autogen.sh --enable-debug --enable-debug-symbols=full --disable-ssl --enable-trace --quiet
				abort_on_failure $?
			popd

		popd # src

		./autogen.sh --enable-debug --enable-debug-symbols=full --with-libbson=bundled --disable-ssl --enable-trace --quiet
		abort_on_failure $?

		make -s -j2 all
		abort_on_failure $?

		sudo make install
		abort_on_failure $?

	popd # libmongoc
popd # src



phpize
./configure --enable-coverage
abort_on_failure $?

make all
abort_on_failure $?

sudo make install
abort_on_failure $?




rm -rf tmp-lcov src/libmongoc # coveralls may pick it up and lie about our coverage

echo "extension=phongo" >> `php --ini | grep "Loaded Configuration" | sed -e "s|.*:\s*||"`

MONGO=`which mongo`
mongod --version
ls $MONGO*
pwd


