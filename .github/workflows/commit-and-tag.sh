#!/usr/bin/env bash
set -e

PACKAGE_VERSION=$1
GPG_KEY_ID=$2

gpgloader

# Create signed "Package x.y.z" commit
echo "Create package commit"
git commit -m "Package ${PACKAGE_VERSION}" -s --gpg-sign=${GPG_KEY_ID} phongo_version.h

# Create signed "Release x.y.z" tag
echo "Create release tag"
git tag -m "Release ${PACKAGE_VERSION}" -s --local-user=${GPG_KEY_ID} ${PACKAGE_VERSION}
