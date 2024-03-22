# Evergreen

See [Evergreen Build System Architecture](architecture.md) for more details.

## mongo-php-toolchain

[10gen/mongo-php-toolchain](https://github.com/10gen/mongo-php-toolchain) is
responsible for building PHP binaries that will be distributed to Evergreen
hosts.

## drivers-evergreen-tools

[mongodb-labs/drivers-evergreen-tools](https://github.com/mongodb-labs/drivers-evergreen-tools)
consists of common scripts used by many drivers (e.g. starting
[mongo-orchestration](https://github.com/10gen/mongo-orchestration)). The PHP
driver clones this during the build process.
