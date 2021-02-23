#!/bin/sh

CONFIGURE_OPTS="${CONFIGURE_OPTS} --enable-mongodb-developer-flags --enable-mongodb-coverage"

if [ -n "${SSL_VERSION}" ]; then
    CONFIGURE_OPTS="${CONFIGURE_OPTS} --with-mongodb-ssl=${SSL_VERSION}"
fi

phpize
./configure ${CONFIGURE_OPTS}
