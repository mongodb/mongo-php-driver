# MongoDB PHP Driver

[![Travis Build Status](https://api.travis-ci.org/mongodb/mongo-php-driver.png?branch=master)](https://travis-ci.org/mongodb/mongo-php-driver)
[![AppVeyor Build Status](https://ci.appveyor.com/api/projects/status/gbd3t99ucib5n8sf?svg=true)](https://ci.appveyor.com/project/jmikola/mongo-php-driver)
[![Coverage Status](https://coveralls.io/repos/github/mongodb/mongo-php-driver/badge.svg?branch=master)](https://coveralls.io/github/mongodb/mongo-php-driver?branch=master)

This extension is developed atop the
[libmongoc](https://github.com/mongodb/mongo-c-driver) and
[libbson](https://github.com/mongodb/libbson) libraries. It provides a minimal
API for core driver functionality: commands, queries, writes, connection
management, and BSON serialization.

Userland PHP libraries that depend on this extension may provide higher level
APIs, such as query builders, individual command helper methods, and GridFS.
Application developers should consider using this extension in conjunction with
the [MongoDB PHP library](https://github.com/mongodb/mongo-php-library), which
implements the same higher level APIs found in MongoDB drivers for other
languages (as well as the
[legacy PHP driver](https://php.net/manual/en/book.mongo.php)).

## Documentation

 - https://php.net/manual/en/set.mongodb.php
 - https://docs.mongodb.com/ecosystem/drivers/php/

## Installation

To build and install the driver:

    $ pecl install mongodb
    $ echo "extension=mongodb.so" >> `php --ini | grep "Loaded Configuration" | sed -e "s|.*:\s*||"`

Additional installation instructions may be found in the
[PHP.net documentation](https://php.net/manual/en/mongodb.installation.php).

This extension is intended to be used alongside the
[MongoDB PHP Library](https://github.com/mongodb/mongo-php-library), which is
distributed as the
[`mongodb/mongodb`](https://packagist.org/packages/mongodb/mongodb) package for
for [Composer](https://getcomposer.org).

## Reporting Issues

Issues pertaining to the extension should be reported in the
[PHPC](https://jira.mongodb.org/secure/CreateIssue!default.jspa?project-field=PHPC)
project in MongoDB's JIRA. Library-related issues should be reported in the
[PHPLIB](https://jira.mongodb.org/secure/CreateIssue!default.jspa?project-field=PHPLIB)
project.

For general questions and support requests, please use one of MongoDB's
[Technical Support](https://docs.mongodb.com/manual/support/) channels.

### Security Vulnerabilities

If you've identified a security vulnerability in a driver or any other MongoDB
project, please report it according to the instructions in
[Create a Vulnerability Report](https://docs.mongodb.org/manual/tutorial/create-a-vulnerability-report).

## Development

Development is tracked in the
[PHPC](https://jira.mongodb.org/projects/PHPC/summary) project in MongoDB's
JIRA. Documentation for contributing to this project may be found in
[CONTRIBUTING.md](CONTRIBUTING.md).
