#!/usr/bin/env bash
set -o errexit  # Exit the script with error if any of the commands fail

# Supported environment variables
API_VERSION=${API_VERSION:-} # Optional API_VERSION environment variable for run-tests.php
CRYPT_SHARED_LIB_PATH="${CRYPT_SHARED_LIB_PATH:-}" # Optional path to crypt_shared library
MONGODB_URI=${MONGODB_URI:-} # Connection string (including credentials and topology info)
APPEND_URI=${APPEND_URI:-} # Additional URI options to append to the URI. Used in OCSP tests
SKIP_CRYPT_SHARED="${SKIP_CRYPT_SHARED:-no}" # Specify "yes" to ignore CRYPT_SHARED_LIB_PATH. Defaults to "no"
SSL=${SSL:-no} # Specify "yes" to enable SSL. Defaults to "no"
SSL_DIR=${SSL_DIR-} # Optional SSL_DIR environment variable for run-tests.php
TESTS=${TESTS:-} # Optional TESTS environment variable for run-tests.php

OS=$(uname -s | tr '[:upper:]' '[:lower:]')
[ -z "$MARCH" ] && MARCH=$(uname -m | tr '[:upper:]' '[:lower:]')

export REPORT_EXIT_STATUS=1

# Append test-specific URI options
MONGODB_URI=${MONGODB_URI}${APPEND_URI}

# Determine if MONGODB_URI already has a query string
SUFFIX=$(echo "$MONGODB_URI" | grep -Eo "\?(.*)" | cat)

if [ "$SSL" = "yes" ]; then
   if [ -z "$SUFFIX" ]; then
      MONGODB_URI="${MONGODB_URI}/?ssl=true&sslallowinvalidcertificates=true"
   else
      MONGODB_URI="${MONGODB_URI}&ssl=true&sslallowinvalidcertificates=true"
   fi
fi

if [ "${SKIP_CRYPT_SHARED}" = "yes" ]; then
   CRYPT_SHARED_LIB_PATH=""
   echo "crypt_shared library is skipped"
elif [ -z "${CRYPT_SHARED_LIB_PATH}" ]; then
   echo "crypt_shared library path is empty"
else
   echo "crypt_shared library will be loaded from path: $CRYPT_SHARED_LIB_PATH"
fi

echo "Running tests with URI: $MONGODB_URI"

# Run the tests, and store the results in a junit result file
API_VERSION="${API_VERSION}" \
CRYPT_SHARED_LIB_PATH="${CRYPT_SHARED_LIB_PATH}" \
MONGODB_URI="${MONGODB_URI}" \
SSL_DIR="${SSL_DIR}" \
TEST_PHP_JUNIT="${PROJECT_DIRECTORY}/test-results.xml" \
TEST_PHP_ARGS="-q -x --show-diff -g FAIL,XFAIL,BORK,WARN,LEAK,SKIP" \
TESTS="$TESTS" \
make test-no-build
