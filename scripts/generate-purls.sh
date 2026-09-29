#!/usr/bin/env bash

# Writes the purls for the bundled dependencies to stdout, derived from the
# version files that accompany the submodules. Shared by update-sbom.sh, which
# feeds them to silkbomb, and by the check-sbom workflow, which compares them
# against the committed sbom.json.

set -euo pipefail

SCRIPT_DIR=$(dirname "${BASH_SOURCE[0]}")
ROOT_DIR=$(realpath "${SCRIPT_DIR}/../")

LIBMONGOC_VERSION=$(cat "${ROOT_DIR}/src/libmongoc/VERSION_CURRENT" | tr -d '[:space:]')
LIBMONGOCRYPT_VERSION=$(cat "${ROOT_DIR}/src/LIBMONGOCRYPT_VERSION_CURRENT" | tr -d '[:space:]')

echo "pkg:github/mongodb/mongo-c-driver@${LIBMONGOC_VERSION}"
echo "pkg:github/mongodb/libmongocrypt@${LIBMONGOCRYPT_VERSION}"
