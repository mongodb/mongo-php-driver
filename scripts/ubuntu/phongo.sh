apt-get install -y php-pear php5-dbg gdb
apt-get install -y libssl-dev libsasl2-dev libpcre3-dev pkg-config

ls -1 /phongo/mongodb*.tgz | sort -n -r | xargs sudo pecl install -f 2>&1 > /phongo/.build
php -m | grep -q mongodb || echo "extension=mongodb.so" >> `php --ini | grep "Loaded Configuration" | sed -e "s|.*:\s*||"`
pecl run-tests -q -p mongodb 2>&1 > /phongo/.tests
