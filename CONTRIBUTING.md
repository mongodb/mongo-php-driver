# Contributing to pecl/MongoDB


## Building from VCS

Developers who would like to contribute to the driver will need to build it from VCS
like so:

```
$ git clone https://github.com/10gen-labs/mongo-php-driver-prototype.git phongo
$ cd phongo
$ git submodule update --init
$ phpize
$ ./configure --enable-developer-flags
$ make -j8 all
```

Be aware that we are submoduling the `1.2-dev` branch of libmongoc and libbson as
during the development of phongo there are occasional things we require that simply
are not yet in a release (or even finished) :)


