# pecl/mongodb (MongoDB driver for PHP)

[![Build Status](https://api.travis-ci.org/mongodb/mongo-php-driver.png?branch=master)](https://travis-ci.org/mongodb/mongo-php-driver)
[![Coverage Status](https://coveralls.io/repos/mongodb/mongo-php-driver/badge.svg?branch=master&service=github)](https://coveralls.io/github/mongodb/mongo-php-driver?branch=master)

This is the low-level PHP driver for MongoDB. The API is the same
as the HHVM driver for MongoDB. The documentation for both of them is the same,
and can be found at http://docs.php.net/manual/en/set.mongodb.php

The driver is written to be a bare bone layer to talk to MongoDB, and
therefore misses many convenience features. Instead, these convenience methods
have been split out into a layer written in PHP, the
[MongoDB Library](http://mongodb.github.io/mongo-php-library/).
Using this library should be your preferred way of interacting with MongoDB.

Please note that the new HHVM and PHP drivers implement a **different API**
from the legacy driver at http://pecl.php.net/package/mongo; therefore 
existing libraries that use the legacy driver (e.g. 
[Doctrine MongoDB's ODM](http://doctrine-mongodb-odm.readthedocs.org/en/latest/))
will not work with the new drivers.

In the long run, we hope that userland packages will be built atop this driver
to implement various APIs (e.g. a BC layer for the existing driver, new fluent
interfaces), management utilities (for creating admin utilities and cluster
management applications), and other interesting libraries.

## Documentation

- http://docs.php.net/set.mongodb

## Installation

To build and install the driver:

```
$ pecl install mongodb
$ echo "extension=mongodb.so" >> `php --ini | grep "Loaded Configuration" | sed -e "s|.*:\s*||"`
```

We recommend using this extension in conjunction with our
[userland library](https://github.com/mongodb/mongo-php-library),
which is distributed as
[mongodb/mongodb](https://packagist.org/packages/mongodb/mongodb) for Composer.

## Contributing

See [CONTRIBUTING.md](CONTRIBUTING.md)

## Related Projects
- [HHVM Implementation of this driver](https://github.com/mongodb/mongo-hhvm-driver)
- [Official high-level library](https://github.com/mongodb/mongo-php-library)
- [MongoDB Transistor](https://github.com/bjori/mongo-php-transistor) Lightweight ODM using the [Persistable](http://php.net/bson\\persistable) interface

