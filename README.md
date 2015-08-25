# pecl/mongodb (MongoDB driver for PHP)

[![Build Status](https://api.travis-ci.org/10gen-labs/mongo-php-driver-prototype.png?branch=PHP7)](https://travis-ci.org/10gen-labs/mongo-php-driver-prototype)
[![Coverage Status](https://coveralls.io/repos/10gen-labs/mongo-php-driver-prototype/badge.svg?branch=PHP7)](https://coveralls.io/r/10gen-labs/mongo-php-driver-prototype?branch=PHP7)

This is branch is for the development of the PHP7 version of phongo.
It is under active development but is not expected to work just yet, or even compile.
If you are interested in bleeeeeding edge, and are comfortable with doing lot of work yourself
to get this working the feel free to give it a shot!

## Documentation
- http://docs.php.net/set.mongodb
- http://10gen-labs.github.io/mongo-php-driver-prototype

## Installation

To build and install the driver:

```
$ git clone -b PHP7 https://github.com/10gen-labs/mongo-php-driver-prototype.git
$ cd mongo-php-driver-prototype
$ git submodule update --init
$ phpize
$ ./configure
$ make -j6 all
$ make install
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

