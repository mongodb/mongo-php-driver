# pecl/mongodb (MongoDB driver for PHP)

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
$ pecl install mongodb-alpha
$ echo "extension=mongodb.so" >> `php --ini | grep "Loaded Configuration" | sed -e "s|.*:\s*||"`
```

## Contributing

See [CONTRIBUTING.md](CONTRIBUTING.md)

## Testing

To execute the driver's test suite:

```
$ make vm # requires vagrant (www.vagrantup.com)
$ make test-bootstrap # Spins up mongod's in the virtual machines
$ make test # Executes the test suite against the virtual machines
```

Some tests depend on userland PHP libraries (e.g. creating data fixtures) and
may be skipped if those dependencies are not available. Those libraries may be
installed with via Composer:

```
$ make composer
```
