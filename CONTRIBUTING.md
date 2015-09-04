# Contributing to pecl/MongoDB


## Building from VCS

Developers who would like to contribute to the driver will need to build it from VCS
like so:

```
$ git clone https://github.com/mongodb-labs/mongo-php-driver-prototype.git phongo
$ cd phongo
$ git submodule update --init
$ phpize
$ ./configure --enable-developer-flags
$ make -j8 all
```

Be aware that we are submoduling the `1.2-dev` branch of libmongoc and libbson as
during the development of phongo there are things we require that simply
are not yet in a release (or even finished) :)


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
The location of this PHONGO-SERVERS.json file can be configured by exporting
`PHONGO_SERVERS` environment variable with a absolute path to the json configuration file.

Alternatively, we provide, and use, [Vagrant](https://www.vagrantup.com/) to spin up handful of VMs
where we setup and configure MongoDB according to our needs.
This is the preferred way of creating PHONGO-SERVERS.json and running the test suite.


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


