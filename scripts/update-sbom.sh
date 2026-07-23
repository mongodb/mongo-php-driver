#!/usr/bin/env bash

set -euo pipefail

SCRIPT_DIR=$(dirname "${BASH_SOURCE[0]}")
ROOT_DIR=$(realpath "${SCRIPT_DIR}/../")
PURLS_FILE="${ROOT_DIR}/purls.txt"
trap 'rm -f "$PURLS_FILE"' EXIT

LIBMONGOC_VERSION=$(cat "${ROOT_DIR}/src/libmongoc/VERSION_CURRENT" | tr -d '[:space:]')
LIBMONGOCRYPT_VERSION=$(cat "${ROOT_DIR}/src/LIBMONGOCRYPT_VERSION_CURRENT" | tr -d '[:space:]')

# Generate purls file from stored versions
echo "pkg:github/mongodb/mongo-c-driver@${LIBMONGOC_VERSION}" > "$PURLS_FILE"
echo "pkg:github/mongodb/libmongocrypt@${LIBMONGOCRYPT_VERSION}" >> "$PURLS_FILE"

# Log in to the DevProd Platforms ECR registry that hosts silkbomb. Requires membership in the
# devprod-platforms-ecr-users Okta group and an AWS SSO profile for the account; see
# https://docs.devprod.prod.corp.mongodb.com/devprod-platforms-ecr#from-your-laptop
profile="${DEVPROD_PLATFORMS_ECR_AWS_PROFILE:-ECRScopedAccess-901841024863}"
aws ecr get-login-password --region us-east-1 --profile "$profile" | docker login --username AWS --password-stdin 901841024863.dkr.ecr.us-east-1.amazonaws.com

# Use silkbomb to update the sbom.json file
docker run --platform="linux/amd64" -i --rm -v "${ROOT_DIR}:/pwd" \
  901841024863.dkr.ecr.us-east-1.amazonaws.com/release-infrastructure/silkbomb:2.0 \
  update --sbom-in /pwd/sbom.json --purls /pwd/purls.txt --sbom-out /pwd/sbom.json
