#!/usr/bin/env bash

# Writes the purls for the bundled dependencies to stdout, derived from the
# version files that accompany the submodules. Used by the check-sbom workflow,
# which compares them against the committed sbom.json.

set -euo pipefail

SCRIPT_DIR=$(dirname "${BASH_SOURCE[0]}")
ROOT_DIR=$(realpath "${SCRIPT_DIR}/../")

# Note: libmongoc 1.x records its version in the extension repository, while
# libmongoc 2.x keeps a VERSION_CURRENT file inside the submodule.
LIBMONGOC_VERSION=$(cat "${ROOT_DIR}/src/LIBMONGOC_VERSION_CURRENT" | tr -d '[:space:]')
LIBMONGOCRYPT_VERSION=$(cat "${ROOT_DIR}/src/LIBMONGOCRYPT_VERSION_CURRENT" | tr -d '[:space:]')

echo "pkg:github/mongodb/mongo-c-driver@${LIBMONGOC_VERSION}"
echo "pkg:github/mongodb/libmongocrypt@${LIBMONGOCRYPT_VERSION}"
