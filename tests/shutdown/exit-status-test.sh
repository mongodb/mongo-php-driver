#!/usr/bin/env bash

# Check that PHP exits with status 0 when the driver is loaded. A crash during
# process shutdown leaves the script output intact and only changes the exit
# status, so several runs are needed to detect it (PHPC-2803).

set -o errexit
set -o nounset
set -o pipefail

PHP_BINARY="${PHP_BINARY:-php}"
MONGODB_EXTENSION="${MONGODB_EXTENSION:-modules/mongodb.so}"
ITERATIONS="${ITERATIONS:-100}"

MONGODB_EXTENSION="$(cd "$(dirname "$MONGODB_EXTENSION")" && pwd)/$(basename "$MONGODB_EXTENSION")"

# Use the default configuration, which is closer to what users run and makes
# the crash more likely because more extensions are loaded. Fall back to -n when
# the configuration already loads a mongodb extension, so that the extension
# under test is the one loaded. Read the answer from stdout, because an affected
# build can crash during shutdown and change the exit status.
php_args=(-d "extension=$MONGODB_EXTENSION" -d "opcache.enable_cli=1")
default_mongodb=$("$PHP_BINARY" -r 'echo extension_loaded("mongodb") ? "yes" : "no"; fflush(STDOUT);' 2>/dev/null || true)
if [ "$default_mongodb" = "yes" ]; then
    php_args=(-n -d "extension=$MONGODB_EXTENSION" -d "opcache.enable_cli=1")

    # Load opcache explicitly when it is built as a shared extension.
    if ! "$PHP_BINARY" -n -r 'exit(extension_loaded("Zend OPcache") ? 0 : 1);' 2>/dev/null \
        && "$PHP_BINARY" -n -d "zend_extension=opcache" -r 'exit(extension_loaded("Zend OPcache") ? 0 : 1);' 2>/dev/null; then
        php_args+=(-d "zend_extension=opcache")
    fi
fi

# The crash depends on the memory layout, so this is a smoke test. It catches
# the regression in configurations that trigger it, and always passes on a fixed
# build.

# The crash only happens with Cyrus SASL. A build without it would make this
# test pass without checking anything.
php_info="$("$PHP_BINARY" "${php_args[@]}" -i)"
if ! grep -q "libmongoc SASL => enabled" <<< "$php_info"; then
    echo "The driver is not built with Cyrus SASL, so this test would not cover PHPC-2803" >&2
    exit 1
fi

failures=0
for ((i = 0; i < ITERATIONS; i++)); do
    if ! "$PHP_BINARY" "${php_args[@]}" -r 'echo "1\n";' > /dev/null 2>&1; then
        failures=$((failures + 1))
    fi
done

if ((failures > 0)); then
    echo "PHP exited with a non-zero status in $failures of $ITERATIONS runs" >&2
    exit 1
fi

echo "PHP exited with status 0 in $ITERATIONS runs"
