# Contributing to the PHP Driver for MongoDB

## Building from Source

Developers who would like to contribute to the driver will need to build it from
source. The repository may be initialized with:

```
$ git clone https://github.com/mongodb/mongo-php-driver.git
$ cd mongo-php-driver
$ git submodule update --init
```

The following script may be used to build the driver:

```
#!/bin/sh

phpize > /dev/null && \
./configure --enable-mongodb-developer-flags > /dev/null && \
make clean > /dev/null && make all > /dev/null && make install
```

To verify that the installation was successful, run the following command, which
will report `phpinfo()` output for the extension:

```
$ php --ri mongodb
```

## Generating arginfo from stub files

Arginfo structures are generated from stub files using the `gen_stub.php`
file. Note that this requires `phpize` to be run for PHP 8.2 to make use
of all features. After changing a stub file, run `./build/gen_stub.php`
to regenerate the corresponding arginfo files and commit the results.

## Generating function maps for static analysis tools

PHPStan and Psalm use function maps to provide users with correct type analysis
when using this extension. To generate the function map, run the
`generate-function-map` make target. The generated map will be stored in
`scripts/functionMap.php`.

## Testing

The extension's test use the PHPT format from PHP internals. This format is
documented in the following links:

 * [Introduction to PHPT Files](https://qa.php.net/write-test.php)
 * [PHPT - Test File Layout](https://qa.php.net/phpt_details.php)

Generally, most tests will be based on the following template:

```
--TEST--
Description of API or JIRA issue being tested
--SKIPIF--
<?php require __DIR__ . "/../utils/basic-skipif.inc"; ?>
<?php /* One or more skip functions */ ?>
--FILE--
<?php
require_once __DIR__ . "/../utils/basic.inc";

// Test code

?>
===DONE===
<?php exit(0); ?>
--EXPECT--
===DONE===
```

The `basic-skipif.inc` and `basic.inc` files contain utility functions for the
`SKIPIF` and `FILE` sections, respectively. If those functions are not needed
(e.g. skip logic only depends on checking the `PHP_INT_SIZE` constant), the test
should not include the file. When it doubt, keep it simple.

### Best Practices for `SKIPIF`

The [`skipif.php`](tests/utils/skipif.php) file defines various helper functions
for use within a test's [`SKIPIF`](https://qa.php.net/phpt_details.php#skipif_section)
section. When multiple functions are used in a single `SKIPIF` section, they
should be logically ordered:

 * Any PHP environment requirements should be checked first. For example, if a
   test requires a 64-bit architecture, start by checking `PHP_INT_SIZE` before
   anything else.
 * Any extension build requirements (e.g. `skip_if_not_libmongoc_crypto()`) or
   test environment requirements (e.g. `skip_if_auth()`) should then be checked.
   These functions only examine local information, such as `phpinfo()` output or
   the structure of the `URI` constant, and do not interact with a remote
   MongoDB server.
 * Any remote server requirements should then be checked. A general integration
   test that requires any type of remote server to be accessible might use
   `skip_if_not_live()` while a test requiring a replica set would prefer
   `skip_if_not_replica_set()`.
 * After requiring a remote server to be accessible (optionally with a specific
   type), you can enforce requirements about that server. This includes checking
   its server version, storage engine, availability of test commands, etc.
 * Finally, use `skip_if_not_clean()` if needed to ensure that the collection(s)
   under test are dropped before the test runs.

As a rule of thumb, your `SKIPIF` logic should be written to allow the test to
run in as many environments as possible. To paraphrase the
[robustness principal](https://en.wikipedia.org/wiki/Robustness_principle):

> Be conservative in what/how you test, and liberal in what environment you require

Consider that a well-crafted `EXPECTF` section may allow a `SKIPIF` section to
be less restrictive.

### Environment Variables

The test suite references the following environment variables:

 * `MONGODB_DATABASE`: Default database to use in tests. Defaults to `phongo`.
 * `MONGO_ORCHESTRATION_URI`: API endpoint for
   [Mongo Orchestration](https://github.com/10gen/mongo-orchestration). Defaults
   to `http://localhost:8889/v1`. This is only used by a few tests that start
   temporary servers, and those tests will be skipped if Mongo Orchestration is
   inaccessible.
 * `MONGODB_URI`: Connection string. Defaults to `mongodb://127.0.0.1/`, which
   assumes a MongoDB server is listening on localhost port 27017.
 * `SSL_DIR`: Path to directory containing certificate files. On Evergreen, this
   will be set to the
   [.evergreen/x509gen](https://github.com/mongodb-labs/drivers-evergreen-tools/tree/master/.evergreen/x509gen)
   directory within
   [drivers-evergreen-tools](https://github.com/mongodb-labs/drivers-evergreen-tools).
   If undefined or inaccessible, tests requiring certificates will be skipped.

The following environment variable is used for [stable API testing](https://github.com/mongodb/specifications/blob/master/source/versioned-api/tests/README.rst):

 * `API_VERSION`: If defined, this value will be used to construct a
   [`MongoDB\Driver\ServerApi`](https://www.php.net/manual/en/mongodb-driver-serverapi.construct.php),
   which will then be specified as the `serverApi` driver option for
   [`MongoDB\Driver\Manager`](https://www.php.net/manual/en/class.mongodb-driver-manager.php)
   objects created by the test suite.

The following environment variables are used for [CSFLE testing](https://github.com/mongodb/specifications/blob/master/source/client-side-encryption/tests/README.rst):

 * `CRYPT_SHARED_LIB_PATH`: If defined, this value will be used to set the
   `cryptSharedLibPath` autoEncryption driver option for
   [`MongoDB\Driver\Manager`](https://www.php.net/manual/en/class.mongodb-driver-manager.php)
   objects created by the test suite.

### Mongo Orchestration

[Mongo Orchestration](https://github.com/10gen/mongo-orchestration) is an HTTP
server that provides a REST API for managing MongoDB servers and clusters.
Evergreen CI and GitHub Actions use configurations provided by the
[drivers-evergreen-tools](https://github.com/mongodb-labs/drivers-evergreen-tools)
repository. These configurations are loaded by Mongo Orchestration, which then
provides a connection string to assign to `MONGODB_URI` and run the test suite.
Additionally, some tests start temporary servers and interact directly with
Mongo Orchestration (via `MONGO_ORCHESTRATION_URI`).

For local development, running Mongo Orchestration is not required and it is
generally sufficient to test against a single-node replica set.

## Updating libmongoc, libbson, and libmongocrypt

The PHP driver can use either system libraries or bundled versions of
libmongoc, libbson, and libmongocrypt. If a new version of either library is
available, the submodule and build configuration will need to be updated to
reflect the new sources and/or package version.

### Resolving Submodule Conflicts When Merging Maintenance Branches

When merging a maintenance branch up to master, it is quite possible for there
to be a submodule conflict (and in related files). Resolving a merge conflict
should never result in downgrading a submodule to an older version. Rather, the
submodule should be conservatively advanced and satisfy any requirements for
incoming changes from the maintenance branch.

For example, the 1.14.2 release bumped libmongoc from 1.22.1 to 1.22.2. When
merging v1.14 into master (1.15-dev), a conflict arose because the libmongoc
submodule in master pointed to a 1.23-dev commit (before the 1.23.0 release).
In this case, there were no particular requirements for incoming changes from
the v1.14 branch and the libmongoc submodule was bumped to 1.23.0 even though
a 1.23.1 tag also existed at the time. The bump to libmongoc 1.23.1 was left to
another PHPC ticket in the 1.15.0 milestone, which actually depended on the
libmongoc changes therein.

### Updating bundled libraries

The following steps are the same for libmongoc and libmongocrypt. When updating
libmongocrypt, follow the same steps but replace `libmongoc` with
`libmongocrypt`, retaining the same capitalization. The following examples
always refer to libmongoc.

#### Update submodule

```shell
cd src/libmongoc
git fetch
git checkout 1.20.0
```

During development, it may be necessary to temporarily point the submodule to a
commit on the developer's fork. For instance, the developer may be working on a
PHP driver feature that depends on unmerged or unreleased changes. In this case,
the submodule path can be updated using the `git submodules set-url` command can
be used to change the URL, and `git submodules set-branch` can be used to point
the submodule to a development branch:

```shell
git submodules set-url src/libmongoc https://github.com/<owner>/<repo>.git
git submodules set-branch -b <branch> src/libmongoc
```

#### Ensure version information is correct

Various build processes and tools rely on the version files to infer version
information. This file can be regenerated using Makefile targets:

```shell
make libmongoc-version-current
```

Alternatively, the `build/calc_release_version.py` script in the submodule can
be executed directly.

Note: If the submodule points to a non-release, non-master branch, the script
may fail to correctly detect the version. This issue is being tracked in
[CDRIVER-3315](https://jira.mongodb.org/browse/CDRIVER-3315) and can be safely ignored since this should only happen
during development (any PHP driver release should point to a tagged submodule
version).

#### Update sources in build configurations

The Autotools and Windows build configurations (`config.m4` and `config.w32`,
respectively) define several variables (e.g. `PHP_MONGODB_MONGOC_SOURCES`) that
collectively enumerate all of the sources within the submodules to include in a
bundled build.

These variables should each have a shell command in a preceding comment, which
should be run to regenerate that particular list of source files. Each command
may be run manually or `scripts/update-submodule-sources.php` may be used to
update all variables. In the event that a new source directory is introduced,
this directory will need to be manually added following prior art.

#### Update package dependencies

The Autotools configuration additionally includes some `pkg-config` commands for
using libmongoc, libbson, and libmongocrypt as system libraries (in lieu of a 
bundled build). When bumping the bundled version, be sure to update the version
check _and_ error message in the `pkg-config` blocks for the submodule being
updated. When updating libmongoc, be sure to update both version checks for
libmongoc and libbson.

For example, the following lines might be updated for libmongoc:

```
if $PKG_CONFIG libmongoc-1.0 --atleast-version 1.20.0; then

...

AC_MSG_ERROR(system libmongoc must be upgraded to version >= 1.20.0)
```

#### Update tested versions in Evergreen configuration (libmongoc only)

Evergreen tests against multiple versions of libmongoc. When updating to a newer
libmongoc version, make sure to update the libmongoc build tasks in `.evergreen/config/templates/build/build-libmongoc.yml`
and regenerate the build configuration. The template file contains additional
information about the build tasks and where they are used. In general, we test
against two additional versions of libmongoc:

- The upcoming patch release of the current libmongoc minor version (e.g. the
  `r1.x` branch)
- The upcoming minor release of libmongoc (e.g. the `master` branch)

#### Update sources in PECL package generation script

If a new version of a submodule introduces a new source directory, that may also
require updating the glob patterns in the `bin/prep-release.php` script to
ensure new source files will be included in any generated PECL package.

#### Update SBOM file

After updating dependencies, the SBOM file needs to be updated. There is a
script to automate this process:

```shell
./scripts/update-sbom.sh
```

This script will generate a temporary purl file with our dependencies, then run
the internal silkbomb tool to update the SBOM. Note that you need to have docker
installed in order to run this.

#### Test and commit your changes

Verify that the upgrade was successful by ensuring that the driver can compile
using both the bundled sources and system libraries, and by ensuring that the
test suite passes. Once done, commit the changes to all of the above
files/paths. For example:

```shell
git commit -m "Bump libmongoc to 1.20.0" config.m4 config.w32 src/libmongoc src/LIBMONGOC_VERSION_CURRENT sbom.json
```
