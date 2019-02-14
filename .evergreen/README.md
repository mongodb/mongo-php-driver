# Evergreen Notes

## mongo-php-toolchain

[10gen/mongo-php-toolchain](https://github.com/10gen/mongo-php-toolchain) is
responsible for building PHP binaries that will be distributed to Evergreen
hosts.

The Debian and RHEL toolchain scripts are responsible for building individual
toolchains for each variant (e.g. PHP version, architecture). Debian and RHEL
are separate primarily for reasons related to OpenSSL.

## drivers-evergreen-tools

[mongodb-labs/drivers-evergreen-tools](https://github.com/mongodb-labs/drivers-evergreen-tools)
consists of common scripts used by many drivers (e.g. Mongo Orchestration). The
PHP driver clones this during the build process.

In some cases, we have modified scripts in the drivers-evergreen-tools
repository. For instance, the PHP driver uses its own `run-orchestration.sh`
script but still relies on the common `stop-orchestration.sh` script.

## PHP driver Evergreen configuration

`$PROJECT_DIRECTORY` is set by Evergreen and denotes the directory where the PHP
driver is built and tested (i.e. git checkout directory).

Various files have been copied and modified from drivers-evergreen-tools:

 * `compile.sh`: This is the main entry point for building the driver and will
   either call `compile-unix.sh` and `compile-windows.sh`; however, Windows
   builds use a Cygwin environment and are not yet supported.
 * `compile-unix.sh`: This includes architecture-specific flags and is used for
   both Linux and macOS builds.
 * `run-orchestration.sh`: This is very similar to the drivers-evergreen-tools
   script but it is customized to use the PHP driver's own topology configs for
   Mongo Orchestration.
 * `start-orchestration.sh`: This is modified to install an older, tagged
   version of Mongo Orchestration. This was necessary for a particular platform,
   but may be something to look into in the future.

The PHP driver's `config.yml` is a very stripped down and heavily modified copy
of the drivers-evergreen-tools configuration. This file starts by defining
several functions, which consist of one or more commands (types supported by
Evergreen).

In the case of the `bootstrap mongo-orchestration` function, we execute our
modified `run-orchestration.sh` script and assign various environment variables
which have been defined by our matrix configuration (excluding the
`$PROJECT_DIRECTORY`, which is set by Evergreen itself).

In `run tests`, we also depend on environment variables (e.g. `$MONGODB_URI`)
that originates in `run-orchestration.sh` and are unpacked and assigned by the
`expansions.update` command within the `bootstrap mongo-orchestration` function.
Since Evergreen functions do not return values, assigning environment variables
is the preferred way to communicate down the line.

The `pre` and `post` block define a sequence of functions to run before and
after each build, respectively.

Build variants are the top-level scope in Evergreen. They consist of a list of
tasks (i.e. a square in Evergreen output) and a matrix. The matrix specification
is the combination of all included axes. For instance, the `tests-php5` matrix
may consist of all OS types with PHP5, MongoDB server version 4.0 (we need not
test all server versions), and lastly one or more specific PHP 5.x versions.
Such a matrix might yield a dozen variants, which will be run for each task in
the build variant configuration. The `display_name` option determines how the
build variant (combination of matrix and all tasks) is labeled in the Evergreen
UI.

The `axes` block defines a list of values for a specific group or ID, which is
used when defining a matrix. For instance, if a matrix includes 4 axes, it will
expand to all possible combinations of values within those axes. Care should be
taken to not create too many redundant combinations when defining matrices.

The `exclude_spec` field within a build variant allows us to exclude one or more
combinations from the generated matrix. We typically use this to exclude an
environment we know is not supported (e.g. MongoDB 3.0 and 3.2 on zSeries).
