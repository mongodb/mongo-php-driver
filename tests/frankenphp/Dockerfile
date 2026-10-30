# Builds the extension from this checkout into the FrankenPHP image, so the
# functional test runs against the PHP build that FrankenPHP embeds (ZTS).
FROM dunglas/frankenphp:1.13.4-php8.5

# Dependencies needed by the bundled libmongoc and libmongocrypt builds.
RUN apt-get update && \
    apt-get install --yes --no-install-recommends cmake libssl-dev libsasl2-dev && \
    rm -rf /var/lib/apt/lists/*

COPY config.m4 Makefile.frag phongo.c phongo.h phongo_version.h /usr/src/mongodb/
COPY scripts /usr/src/mongodb/scripts
COPY src /usr/src/mongodb/src

WORKDIR /usr/src/mongodb

RUN phpize && \
    ./configure --with-php-config="$(command -v php-config)" && \
    make -j "$(nproc)" && \
    make install && \
    docker-php-ext-enable mongodb

COPY tests/frankenphp/Caddyfile /etc/frankenphp/Caddyfile
COPY tests/frankenphp/worker.php /app/public/worker.php
