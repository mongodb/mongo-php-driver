ls -1 /phongo/mongodb*.tgz | sort -n -r | xargs sudo pecl install -f 2>&1 > /phongo/.build
if test $? -eq 0; then
	php -m | grep -q mongodb || echo "extension=mongodb.so" | sudo tee -a /usr/local/etc/php/extensions.ini
	pecl run-tests -q -p mongodb 2>&1 > /phongo/.tests
else
	tail -n50 /phongo/.build
	exit 3
fi


