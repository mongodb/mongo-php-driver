#!/usr/bin/env bash
set -e

gpgloader

FILENAME=$1

gpg --yes -v --armor -o "${FILENAME}.sig" --detach-sign "${FILENAME}"
