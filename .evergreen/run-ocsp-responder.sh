#!/usr/bin/env bash
set -o errexit

# Run an mock OCSP responder server if necessary. This script should be invoked
# before starting any MongoDB servers.
#
# Environment variables:
#
# TEST_COLUMN
#   Required. Corresponds to a column of the test matrix. Set to one of the
#   following: "TEST_1", "TEST_2", "TEST_3", "TEST_4", "SOFT_FAIL_TEST",
#   "MALICIOUS_SERVER_TEST_1", or "MALICIOUS_SERVER_TEST_2".
#
#   See: https://github.com/mongodb/specifications/blob/master/source/ocsp-support/tests/README.rst#integration-tests-permutations-to-be-tested
#
# CERT_TYPE
#   Required. Set to either "rsa" or "ecdsa".
#
# USE_DELEGATE
#   Optional. May be "ON" or "OFF". If a test requires use of a responder, this
#   determines whether responder uses a delegate certificate. Defaults to "OFF".
#
# DRIVERS_TOOLS
#   Required. Path to clone of drivers-evergreen-tools repository.
#
# PROJECT_DIRECTORY
#   Required. If a test requires use a responder, its output will be logged to a
#   ocsp-responder.log file in this directory.

USE_DELEGATE=${USE_DELEGATE:-OFF}

if [ -z "${CERT_TYPE}" ] || [ -z "${TEST_COLUMN}" ]; then
    echo "CERT_TYPE and TEST_COLUMN are required."
    exit 1
fi

if [ "${CERT_TYPE}" != "rsa" ] && [ "${CERT_TYPE}" != "ecdsa" ]; then
    echo "Unsupported value for CERT_TYPE: ${CERT_TYPE}"
    exit 1
fi

case "${TEST_COLUMN}" in
    "TEST_1" | "TEST_3")
        RESPONDER="valid"
        ;;

    "TEST_2" | "TEST_4" | "MALICIOUS_SERVER_TEST_1")
        RESPONDER="invalid"
        ;;

    "SOFT_FAIL_TEST" | "MALICIOUS_SERVER_TEST_2")
        RESPONDER=""
        ;;

    *)
        echo "Unsupported value for TEST_COLUMN: ${TEST_COLUMN}"
        exit 1
        ;;
esac

echo "TEST_COLUMN: ${TEST_COLUMN}"
echo "CERT_TYPE: ${CERT_TYPE}"
echo "USE_DELEGATE: ${USE_DELEGATE}"

if [ -n "$RESPONDER" ]; then
    echo "Starting mock OCSP responder"

    cd ${DRIVERS_TOOLS}/.evergreen/ocsp
    . ./activate-ocspvenv.sh

    if [ "${RESPONDER}" = "invalid" ]; then
        FAULT="--fault revoked"
    fi

    if [ "$USE_DELEGATE" = "ON" ]; then
        SIGNER="ocsp-responder"
    else
        SIGNER="ca"
    fi

    python ocsp_mock.py \
        --ca_file ${CERT_TYPE}/ca.pem \
        --ocsp_responder_cert ${CERT_TYPE}/${SIGNER}.crt \
        --ocsp_responder_key ${CERT_TYPE}/${SIGNER}.key \
        -p 8100 -v $FAULT \
        > ${PROJECT_DIRECTORY}/ocsp-responder.log 2>&1 &
fi
