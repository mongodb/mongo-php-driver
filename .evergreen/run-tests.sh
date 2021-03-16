#!/bin/sh
set -o xtrace   # Write all commands first to stderr
set -o errexit  # Exit the script with error if any of the commands fail

# Supported/used environment variables:
#       SSL                     Set to enable SSL. Defaults to "nossl"
#       MONGODB_URI             Set the suggested connection MONGODB_URI (including credentials and topology info)


AUTH=${AUTH:-noauth}
SSL=${SSL:-nossl}
MONGODB_URI=${MONGODB_URI:-}
TESTS=${TESTS:-}
API_VERSION=${API_VERSION:-}

OS=$(uname -s | tr '[:upper:]' '[:lower:]')
[ -z "$MARCH" ] && MARCH=$(uname -m | tr '[:upper:]' '[:lower:]')

export REPORT_EXIT_STATUS=1

if [ "$SSL" = "yes" ]; then
   MONGODB_URI="${MONGODB_URI}/?ssl=true&sslallowinvalidcertificates=true"
fi

echo "Running $AUTH tests, connecting to $MONGODB_URI"

# Run the tests, and store the results in a junit result file
case "$OS" in
   *)
      API_VERSION="${API_VERSION}" TEST_PHP_JUNIT="${PROJECT_DIRECTORY}/test-results.xml" TEST_PHP_ARGS="-q -x --show-diff -g FAIL,XFAIL,BORK,WARN,LEAK,SKIP" make test TESTS=$TESTS
      ;;
esac

