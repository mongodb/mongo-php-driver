#!/usr/bin/env bash

SCRIPT_DIR=$(dirname ${BASH_SOURCE[0]})
ROOT_DIR=$(realpath "${SCRIPT_DIR}/../")
PURLS_FILE="${ROOT_DIR}/purls.txt"

LIBMONGOC_VERSION=$(cat ${ROOT_DIR}/src/LIBMONGOC_VERSION_CURRENT | tr -d '[:space:]')
LIBMONGOCRYPT_VERSION=$(cat ${ROOT_DIR}/src/LIBMONGOCRYPT_VERSION_CURRENT | tr -d '[:space:]')

# Generate purls file from stored versions
echo "pkg:github/mongodb/mongo-c-driver@${LIBMONGOC_VERSION}" > $PURLS_FILE
echo "pkg:github/mongodb/libmongocrypt@${LIBMONGOCRYPT_VERSION}" >> $PURLS_FILE

# Use silkbomb to update the sbom.json file
docker run --platform="linux/amd64" -it --rm -v ${ROOT_DIR}:/pwd \
  artifactory.corp.mongodb.com/release-tools-container-registry-public-local/silkbomb:1.0 \
  update --sbom-in /pwd/sbom.json --purls /pwd/purls.txt --sbom-out /pwd/sbom.json

rm $PURLS_FILE
