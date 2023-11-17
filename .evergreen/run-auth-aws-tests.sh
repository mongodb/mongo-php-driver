#!/usr/bin/env bash
set -o errexit  # Exit the script with error if any of the commands fail
set -o pipefail # Exit the script with error if any piped commands fail
#set +o xtrace # Don't trace commands

# Supported environment variables
PROJECT_DIRECTORY=${PROJECT_DIRECTORY:-} # Path to project directory
DRIVERS_TOOLS=${DRIVERS_TOOLS:-} # Path to clone of drivers-evergreen-tools repository
MONGODB_URI=${MONGODB_URI:-} # Connection string
TESTCASE=${TESTCASE:-} # Test case to run (e.g. ASSUME_ROLE_WITH_WEB_IDENTITY)

# Some of the setup scripts expect mongo to be on path.
export PATH
PATH+=":${PROJECT_DIRECTORY}/mongodb/bin"

# Determine if MONGODB_URI already has a query string
SUFFIX=$(echo "$MONGODB_URI" | grep -Eo "\?(.*)" | cat)

if [ -z "$SUFFIX" ]; then
  MONGODB_URI="${MONGODB_URI}/?authMechanism=MONGODB-AWS"
else
  MONGODB_URI="${MONGODB_URI}&authMechanism=MONGODB-AWS"
fi

export MONGODB_URI
export TEST_PHP_ARGS="-q -x --show-diff -g FAIL,XFAIL,BORK,WARN,LEAK,SKIP"

if [[ "${TESTCASE}" == "ASSUME_ROLE_WITH_WEB_IDENTITY" ]]; then
  echo "===== Testing auth via Web Identity ====="
  # Do necessary setup.
  # Create user on $external db.
  pushd "${DRIVERS_TOOLS}/.evergreen/auth_aws"
  mongo --verbose aws_e2e_web_identity.js
  popd # "${DRIVERS_TOOLS}/.evergreen/auth_aws"

  declare iam_auth_assume_web_role_name iam_web_identity_token_file
  iam_auth_assume_web_role_name="$(jq -r '.iam_auth_assume_web_role_name' "${DRIVERS_TOOLS}/.evergreen/auth_aws/aws_e2e_setup.json")"
  iam_web_identity_token_file="$(jq -r '.iam_web_identity_token_file' "${DRIVERS_TOOLS}/.evergreen/auth_aws/aws_e2e_setup.json")"

  echo "Valid credentials via Web Identity - should succeed"
  AWS_ROLE_ARN="${iam_auth_assume_web_role_name}" \
  AWS_WEB_IDENTITY_TOKEN_FILE="${iam_web_identity_token_file}" \
  TESTS="tests/auth-success.phpt" \
  make test

  echo "Valid credentials via Web Identity with session name - should succeed"
  AWS_ROLE_ARN="${iam_auth_assume_web_role_name}" \
  AWS_WEB_IDENTITY_TOKEN_FILE="${iam_web_identity_token_file}" \
  AWS_ROLE_SESSION_NAME="test" \
  TESTS="tests/auth-success.phpt" \
  make test

#  echo "Invalid AWS_ROLE_ARN via Web Identity with session name - should fail"
#  AWS_ROLE_ARN="invalid_role_arn" \
#  AWS_WEB_IDENTITY_TOKEN_FILE="${iam_web_identity_token_file}" \
#    expect_failure "mongodb://localhost/?authMechanism=MONGODB-AWS"
#
#  echo "Invalid AWS_WEB_IDENTITY_TOKEN_FILE via Web Identity with session name - should fail"
#  AWS_ROLE_ARN="${iam_auth_assume_web_role_name}" \
#  AWS_WEB_IDENTITY_TOKEN_FILE="/invalid/path" \
#    expect_failure "mongodb://localhost/?authMechanism=MONGODB-AWS"
#
#  echo "Invalid AWS_ROLE_SESSION_NAME via Web Identity with session name - should fail"
#  AWS_ROLE_ARN="${iam_auth_assume_web_role_name}" \
#  AWS_WEB_IDENTITY_TOKEN_FILE="${iam_web_identity_token_file}" \
#  AWS_ROLE_SESSION_NAME="contains_invalid_character_^" \
#    expect_failure "mongodb://localhost/?authMechanism=MONGODB-AWS"
  exit
fi

echo "Unexpected testcase '${TESTCASE}'" 1>&2
exit 1
