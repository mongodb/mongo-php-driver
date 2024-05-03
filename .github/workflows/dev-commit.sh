#!/usr/bin/env bash
set -e

gpgloader

git commit -m "Back to -dev" -s --gpg-sign=${GPG_KEY_ID} phongo_version.h
