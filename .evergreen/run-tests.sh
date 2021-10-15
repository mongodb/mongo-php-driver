#!/bin/sh
set -o errexit  # Exit the script with error if any of the commands fail

# Supported/used environment variables:
#   SSL             Set to "yes" to enable SSL. Defaults to "nossl"
#   MONGODB_URI     Set the suggested connection MONGODB_URI (including credentials and topology info)
#   TESTS           Optional TESTS environment variable for run-tests.php
#   API_VERSION     Optional API_VERSION environment variable for run-tests.php
#   MOCK_SERVICE_ID Set to "1" to enable service ID mocking for load balancers. Defaults to empty string.

SSL=${SSL:-nossl}
MONGODB_URI=${MONGODB_URI:-}
TESTS=${TESTS:-}
API_VERSION=${API_VERSION:-}
MOCK_SERVICE_ID=${MOCK_SERVICE_ID:-}

OS=$(uname -s | tr '[:upper:]' '[:lower:]')
[ -z "$MARCH" ] && MARCH=$(uname -m | tr '[:upper:]' '[:lower:]')

export REPORT_EXIT_STATUS=1

# Determine if MONGODB_URI already has a query string
SUFFIX=$(echo "$MONGODB_URI" | grep -Eo "\?(.*)" | cat)

if [ "$SSL" = "yes" ]; then
   if [ -z "$SUFFIX" ]; then
      MONGODB_URI="${MONGODB_URI}/?ssl=true&sslallowinvalidcertificates=true"
   else
      MONGODB_URI="${MONGODB_URI}&ssl=true&sslallowinvalidcertificates=true"
   fi
fi

echo "Running tests with URI: $MONGODB_URI"

# Run the tests, and store the results in a junit result file
case "$OS" in
   *)
      MOCK_SERVICE_ID="${MOCK_SERVICE_ID}" API_VERSION="${API_VERSION}" TEST_PHP_JUNIT="${PROJECT_DIRECTORY}/test-results.xml" TEST_PHP_ARGS="-q -x --show-diff -g FAIL,XFAIL,BORK,WARN,LEAK,SKIP" make test TESTS=$TESTS
      ;;
esac

