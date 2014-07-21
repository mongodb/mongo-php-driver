#!/bin/sh

PHP=/home/travis/.phpenv/versions/`php -r 'echo PHP_VERSION;'`/bin/php
echo "backtrace full" | gdb $PHP $1

