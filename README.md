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

The test suites expects to find `PHONGO-SERVERS.json` in the system temp directory (typically `/tmp`).
This file should contain JSON object with MONGODB URIs, similar to the following template

```
{
    "STANDALONE": "mongodb:\/\/192.168.112.10:2000",
    "STANDALONE_SSL": "mongodb:\/\/192.168.112.10:2100",
    "STANDALONE_AUTH": "mongodb:\/\/root:toor@192.168.112.10:2200\/?authSource=admin",
    "STANDALONE_X509": "mongodb:\/\/C=US,ST=New York,L=New York City,O=MongoDB,OU=KernelUser,CN=client@192.168.112.10:2300\/?authSource=$external&authMechanism=MONGODB-X509",
    "STANDALONE_PLAIN": "mongodb:\/\/root:toor@192.168.112.10:2400\/?authSource=admin",
    "REPLICASET": "mongodb:\/\/192.168.112.10:3000,192.168.112.10:3001,192.168.112.10:3002\/?replicaSet=REPLICASET"
}
```

Alternatively, we provide, and use, [Vagrant](https://www.vagrantup.com/) to spin up couple of VMs
where we setup and configure MongoDB according to our needs.


```
$ make vm # requires vagrant (www.vagrantup.com)
$ make test-bootstrap # Spins up mongod's in the virtual machines
```

The `test-bootstrap` make target will then generate the required `PHONGO-SERVERS.json`.


To execute the test suite:

```
$ make test # Executes the test suite against the virtual machines
```

The bundled [Vagrantfile](Vagrantfile) also contains few other (growing) list of VMs
that can be provisioned to execute the test suite on various platforms.

These are automatically executed by the `make distcheck`, which will package a new PECL archive,
spin up the various operating systems, install the newly packaged archive, and execute
the test suite.


## Related projects
- [Official high-level library](https://github.com/10gen-labs/mongo-php-library-prototype)
- [MongoDB Transistor](https://github.com/bjori/mongo-php-transistor) Lightweight ODM using the [Persistable](http://php.net/bson\\persistable) interface

