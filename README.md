# MongoDB PHP Driver

[![Tests](https://github.com/mongodb/mongo-php-driver/actions/workflows/tests.yml/badge.svg)](https://github.com/mongodb/mongo-php-driver/actions/workflows/tests.yml)
[![Coverage Status](https://coveralls.io/repos/github/mongodb/mongo-php-driver/badge.svg?branch=master)](https://coveralls.io/github/mongodb/mongo-php-driver?branch=master)
[![Coding Standards](https://github.com/mongodb/mongo-php-driver/actions/workflows/clang-format.yml/badge.svg)](https://github.com/mongodb/mongo-php-driver/actions/workflows/clang-format.yml)

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
languages.

## Documentation

 - https://php.net/manual/en/set.mongodb.php
 - https://www.mongodb.com/docs/drivers/php/

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
[Composer](https://getcomposer.org).

## Release Integrity

Releases are created automatically and signed using the 
[PHP team's GPG key](https://pgp.mongodb.com/php-driver.asc). This applies to
the git tag as well as all release packages provided as part of a
[GitHub release](https://github.com/mongodb/mongo-php-library/releases). To
verify the provided packages, download the key and import it using `gpg`:

```shell
gpg --import php-driver.asc
```

### PECL package

PECL packages are made available as release artifacts on GitHub, as well as on
the [PECL homepage](https://pecl.php.net/mongodb). The GitHub release will also
contain a detached signature file for the PECL package (named
`mongodb-X.Y.Z.tgz.sig`).

To verify the integrity of the downloaded package, run the following command:

```shell
gpg --verify mongodb-X.Y.Z.tgz.sig mongodb-X.Y.Z.tgz
```

> [!NOTE]
> No verification is done when using `pecl` to install the package. To ensure
> release integrity when using `pecl`, download the tarball manually from the
> GitHub release, verify the signature, then install the package from the
> downloaded tarball using `pecl install mongodb-X.Y.Z.tgz`.

### Windows

Windows binaries distributed through GitHub releases contain a detached
signature for the `php_mongodb.dll` file named `php_mongodb.dll.sig`. To verify
the integrity of the DLL, run the following command:

```shell
gpg --verify php_mongodb.dll.sig php_mongodb.dll.tgz
```

> [!NOTE]
> Windows binaries distributed directly through PECL are not signed by MongoDB
> and cannot be verified. If you need to verify the integrity of the downloaded
> binary, always download them from the GitHub release.

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
