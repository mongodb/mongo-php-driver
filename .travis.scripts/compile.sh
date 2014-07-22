#!/bin/bash
trap abort_on_failure ERR

abort_on_failure () {
	echo "Last command failed, exising"
	exit 42;
}

build_libmongoc_mci() {
	if [ ! -d tmp ]; then
		mkdir tmp
	fi

	pushd tmp

		git clone https://github.com/mongodb/mongo-c-driver.git

		pushd mongo-c-driver
			./build/mci.sh  --notest
		popd

		sudo dpkg -i *.deb

	popd
}

build_libmongoc_manually() {

MONGOC_VERSION=master
LIBBSON_VERSION=master
LCOV_VERSION=1.11


mkdir tmp-lcov
pushd tmp-lcov
	wget -O lcov.tar.gz http://downloads.sourceforge.net/ltp/lcov-${LCOV_VERSION}.tar.gz
	tar zxf lcov.tar.gz --strip-components=1
	sudo make install
popd

pushd src
	if [ "$MONGOC_VERSION" = "master" ]; then
		wget -O libmongoc.tar.gz https://github.com/mongodb/mongo-c-driver/archive/master.tar.gz
	else
		wget -O libmongoc.tar.gz https://github.com/mongodb/mongo-c-driver/releases/download/${MONGOC_VERSION}/mongo-c-driver-${MONGOC_VERSION}.tar.gz
	fi


	mkdir libmongoc
	pushd libmongoc
		tar zxf ../libmongoc.tar.gz --strip-components=1

		pushd src
			if [ "$LIBBSON_VERSION" = "master" ]; then
				wget -O libbson.tar.gz https://github.com/mongodb/libbson/archive/master.tar.gz
			else
				wget -O libbson.tar.gz https://github.com/mongodb/libbson/releases/download/${LIBBSON_VERSION}/libbson-${LIBBSON_VERSION}.tar.gz
			fi

			pushd libbson
				tar zxf ../libbson.tar.gz --strip-components=1

				./autogen.sh --enable-debug --enable-debug-symbols=full --disable-ssl --enable-trace --quiet
			popd

		popd # src

		./autogen.sh --enable-debug --enable-debug-symbols=full --with-libbson=bundled --disable-ssl --enable-trace --quiet

		make -s -j2 all
		sudo make install

	popd # libmongoc
popd # src
}



build_libmongoc_mci

phpize
./configure --enable-coverage
make all
sudo make install
echo "Use the most-up-to-date run-tests.. old ones like 5.3 don't report failure exit codes"
wget -O run-tests.php https://raw.githubusercontent.com/php/php-src/master/run-tests.php



rm -rf tmp-lcov tmp src/libmongoc # coveralls may pick it up and lie about our coverage

echo "extension=phongo.so" >> `php --ini | grep "Loaded Configuration" | sed -e "s|.*:\s*||"`

MONGO=`which mongo`
mongod --version
ls $MONGO*
pwd


