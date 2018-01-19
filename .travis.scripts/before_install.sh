#!/bin/sh

if [ "${TRAVIS_OS_NAME}" = "linux" ]; then
    # https://github.com/eddyxu/cpp-coveralls#linux
    pip install --user cpp-coveralls
fi

if [ "${TRAVIS_OS_NAME}" = "osx" ]; then
    brew update
    brew upgrade

    brew tap homebrew/homebrew-php
    brew install "${PHP_VERSION}"

    # http://brewformulas.org/Lcov
    brew install lcov

    # https://github.com/eddyxu/cpp-coveralls#os-x
    brew install pyenv
    eval "$(pyenv init -)"
    pyenv install 2.7.6
    pyenv global 2.7.6
    pyenv rehash
    pip install cpp-coveralls
    pyenv rehash
fi
