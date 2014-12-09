# What is PHongo?

[Phongo](https://github.com/bjori/phongo) is an experimental MongoDB driver for PHP,
likely replacing the
[mongodb/mongo-php-driver](https://github.com/mongodb/mongo-php-driver) driver in the
near future.

Phongo is written ontop of [libmongoc](https://github.com/mongodb/mongo-c-driver) and
[libbson](https://github.com/mongodb/libbson), and is meant to be as basic as possible.
Core MongoDB concepts are implemented, but everything else is left for other toolkits
to come in and provide pretty and flexible bindings that make sense for their ecosystem.

This makes the driver exceptionally fast, and really easy to work with and provide
first class experience in your framework, library or application.

The plan is to provide an example bindings, which you may chose to use - or ignore
and make your own.



# Installation

Ultimately, this extension is not intended to be explicitly installed. Users should
choose one (or more) userland PHP libraries that depend on this driver, and install
them using, for example, Composer.

To build and install the driver:

	$ wget https://github.com/bjori/phongo/releases/download/0.1.2/phongo-0.1.2.tgz
	$ pecl install phongo-0.1.2.tgz
	$ echo "extension=phongo.so" >> `php --ini | grep "Loaded Configuration" | sed -e "s|.*:\s*||"`


