# pecl/mongodb (MongoDB driver for PHP)

[![Build Status](https://api.travis-ci.org/10gen-labs/mongo-php-driver-prototype.png?branch=master)](https://travis-ci.org/10gen-labs/mongo-php-driver-prototype)
[![Coverage Status](https://coveralls.io/repos/10gen-labs/mongo-php-driver-prototype/badge.svg?branch=master)](https://coveralls.io/r/10gen-labs/mongo-php-driver-prototype?branch=master)

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
- http://10gen-labs.github.io/mongo-php-driver-prototype

## Installation

To build and install the driver:

```
$ pecl install mongodb-alpha
$ echo "extension=mongodb.so" >> `php --ini | grep "Loaded Configuration" | sed -e "s|.*:\s*||"`
```

We recommend using this extension in conjunction with our
[userland library](https://github.com/10gen-labs/mongo-php-library-prototype),
which is distributed as
[mongodb/mongodb](https://packagist.org/packages/mongodb/mongodb) for Composer.

## Contributing

See [CONTRIBUTING.md](CONTRIBUTING.md)

## Related projects
- [HHVM Implementation of this driver](https://github.com/10gen-labs/mongo-hhvm-driver-prototype)
- [Official high-level library](https://github.com/10gen-labs/mongo-php-library-prototype)
- [MongoDB Transistor](https://github.com/bjori/mongo-php-transistor) Lightweight ODM using the [Persistable](http://php.net/bson\\persistable) interface

