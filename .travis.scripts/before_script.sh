#!/bin/sh

CONFIGURE_OPTS="--enable-developer-flags --enable-coverage"

if [ -n "${SSL_VERSION}" ]; then
    CONFIGURE_OPTS="${CONFIGURE_OPTS} --with-mongodb-ssl=${SSL_VERSION}"
fi

phpize
./configure ${CONFIGURE_OPTS}
./scripts/clang-format.sh test
if [ ! $? -eq 0 ];
    echo "Formatting errors"
    exit 1;
fi
make all -j4
make install

# Add the extension to php.ini
echo "extension=mongodb.so" >> `php --ini | grep "Loaded Configuration" | sed -e "s|.*:\s*||"`

# Predefine the test server
echo '{"STANDALONE": "mongodb:\/\/127.0.0.1:27017"}' > /tmp/PHONGO-SERVERS.json
