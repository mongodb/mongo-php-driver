# PHongo (PHP MongoDB driver)

[![Coverage Status](https://coveralls.io/repos/bjori/phongo/badge.png?branch=master)](https://coveralls.io/r/bjori/phongo?branch=master)
[![Build Status](https://travis-ci.org/10gen-labs/mongo-php-driver-prototype.svg?branch=master)](https://travis-ci.org/10gen-labs/mongo-php-driver-prototype)
[![Coverity Scan Build Status](https://scan.coverity.com/projects/2600/badge.svg)](https://scan.coverity.com/projects/2600)

> *Note*: This project is highly experimental and various parts are
non-functional. Please do not even think about using this in production.
Development progress is being tracked in the
[pecl-prototype](https://jira.mongodb.org/browse/PHP/component/13249) component
of the MongoDB PHP driver's JIRA project.

The purpose of this driver is to provide a thin glue between MongoDB
and PHP, implementing only fundamental and performance-critical components

In the long run, we hope that userland packages will be built atop this driver
to implement various APIs (e.g. a BC layer for the existing driver, new fluent
interfaces), management utilities (for creating admin utilities and cluster
management applications), and other interesting libraries.

The goal is to encourage community contribution and involvement with these
components (with a lower barrier of entry) while also keeping the overall design
fast and powerful.

## Documentation
- http://10gen-labs.github.io/mongo-php-driver-prototype

## Installation

Ultimately, this extension is not intended to be explicitly installed. Users
should choose one (or more) userland PHP libraries that depend on this driver,
and install them using Composer. At some point in the future, Composer may
support installing extension dependencies.

This project is currently not published as a PECL extension and must be built
and installed manually.

To build and install the driver:

```
$ wget https://github.com/10gen-labs/mongo-php-driver-prototype/releases/download/0.1.3/phongo-0.1.3.tgz
$ pecl install phongo-0.1.3.tgz
$ echo "extension=phongo.so" >> `php --ini | grep "Loaded Configuration" | sed -e "s|.*:\s*||"`
```

## Contributing

See [CONTRIBUTING.md](CONTRIBUTING.md)

## Testing

To execute the driver's test suite:

```
$ make test
```

Some tests depend on userland PHP libraries (e.g. creating data fixtures) and
may be skipped if those dependencies are not available. Those libraries may be
installed with via Composer:

```
$ make composer
```
