# What is pecl/mongodb?

[pecl/mongodb](https://github.com/mongodb/mongo-php-driver) is a new MongoDB
driver for PHP, which will replace the legacy
[pecl/mongo](https://github.com/mongodb/mongo-php-driver-legacy) driver.

This extension is written atop
[libmongoc](https://github.com/mongodb/mongo-c-driver) and
[libbson](https://github.com/mongodb/libbson), and purposefully offers a basic
API for core functionality (e.g. connection handling, queries, commands, and
write operations). Additional functionality is left to be implemented by
userland libraries. Our hope is that this seperation of concerns will keep the
driver lightweight (and fast), easier to maintain, and make it an ideal starting
point for constructing your own framework, library, or application.

# Installation

To build and install the driver:

```
$ pecl install mongodb
$ echo "extension=mongodb.so" >> `php --ini | grep "Loaded Configuration" | sed -e "s|.*:\s*||"`
```

Ultimately, this extension is not intended to be used alone. Users should
considering using this driver alongside one or more userland PHP libraries, such
as [mongo-php-library](https://github.com/mongodb/mongo-php-library).
