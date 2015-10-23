# pecl/mongodb (MongoDB driver for PHP)

[![Build Status](https://api.travis-ci.org/mongodb/mongo-php-driver.png?branch=master)](https://travis-ci.org/mongodb/mongo-php-driver)
[![Coverage Status](https://coveralls.io/repos/mongodb/mongo-php-driver/badge.svg?branch=master&service=github)](https://coveralls.io/github/mongodb/mongo-php-driver?branch=master)

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
- http://docs.php.net/set.mongodb
- http://mongodb.github.io/mongo-php-driver

## Installation

To build and install the driver:

```
$ pecl install mongodb-beta
$ echo "extension=mongodb.so" >> `php --ini | grep "Loaded Configuration" | sed -e "s|.*:\s*||"`
```

We recommend using this extension in conjunction with our
[userland library](https://github.com/mongodb/mongo-php-library),
which is distributed as
[mongodb/mongodb](https://packagist.org/packages/mongodb/mongodb) for Composer.

## Contributing

See [CONTRIBUTING.md](CONTRIBUTING.md)

## Related projects
- [HHVM Implementation of this driver](https://github.com/mongodb/mongo-hhvm-driver)
- [Official high-level library](https://github.com/mongodb/mongo-php-library)
- [MongoDB Transistor](https://github.com/bjori/mongo-php-transistor) Lightweight ODM using the [Persistable](http://php.net/bson\\persistable) interface

