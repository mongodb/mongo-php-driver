# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Overview

This is the low-level MongoDB PHP extension (`mongodb`), written in C. It wraps [libmongoc](https://github.com/mongodb/mongo-c-driver) and [libbson](https://github.com/mongodb/libbson) to expose a minimal PHP API for core driver functionality. Higher-level APIs live in the separate [MongoDB PHP Library](https://github.com/mongodb/mongo-php-library).

Issues are tracked in the [PHPC JIRA project](https://jira.mongodb.org/projects/PHPC). Commit messages and branch names reference JIRA ticket IDs (e.g., `PHPC-2215`).

## Build Commands

```sh
# Initial setup (one-time)
git submodule update --init

# Build and install
phpize
./configure --enable-mongodb-developer-flags
make clean && make all && make install

# Verify installation
php --ri mongodb

# Format C code
make format          # apply clang-format
make format-check    # check only (used in CI)
```

## Testing

```sh
# Run all tests (requires a running MongoDB server on localhost:27017)
make test

# Run a single test file
make test TESTS=tests/path/to/test.phpt

# TESTS accepts a glob pattern:
make test TESTS="tests/path/to/pattern*.phpt"
```

For failed tests, all intermediate files (e.g. expected output, actual output, diff) are left alongside the test file. This is helpful for debugging purposes. 

Key environment variables for testing:
- `MONGODB_URI` – connection string (default: `mongodb://127.0.0.1/`)
- `MONGODB_DATABASE` – test database (default: `phongo`)
- `SSL_DIR` – path to certificate files for TLS tests
- `API_VERSION` – enables Stable API testing
- `CRYPT_SHARED_LIB_PATH` – enables CSFLE testing

## Stub Files and Arginfo

PHP method signatures are defined in `*.stub.php` files alongside each class's `.c` file. After changing a stub file, regenerate the arginfo:

```sh
./build/gen_stub.php
```

This requires `phpize` to have been run with **PHP 8.2** to use all features. Commit both the stub and the generated arginfo file.

## Architecture

### Extension Entry Point

- `phongo.c` / `phongo.h` – module initialization (MINIT/RINIT/MSHUTDOWN/RSHUTDOWN), INI directives, and extension registration.
- `phongo_*.c` files in `src/` – support modules: `phongo_apm`, `phongo_bson`, `phongo_bson_encode`, `phongo_client`, `phongo_error`, `phongo_execute`, `phongo_ini`, `phongo_log`, `phongo_util`, `phongo_atomic`.

### PHP Class Implementations

Each class is implemented as a pair of `.c` / `.h` files (and a `.stub.php` for arginfo):

- **`src/BSON/`** – BSON type wrappers (`Binary`, `ObjectId`, `UTCDateTime`, `Decimal128`, `Regex`, `Timestamp`, `Int64`, `Javascript`, `Document`, `PackedArray`, `VectorType`, etc.) plus their interfaces and serialization interfaces (`Serializable`, `Unserializable`, `Persistable`).
- **`src/MongoDB/`** – Driver classes:
  - `Manager` – main entry point; manages connection pools via libmongoc.
  - `Cursor` – wraps a libmongoc cursor for iterating query/command results.
  - `Session` – client sessions for causal consistency and transactions.
  - `Server`, `ServerDescription`, `TopologyDescription` – server/topology metadata.
  - `Command`, `Query`, `BulkWrite`, `BulkWriteCommand` – operation builders.
  - `ReadPreference`, `WriteConcern`, `ReadConcern`, `ServerApi` – operation options.
  - `WriteResult`, `BulkWriteCommandResult`, `WriteError`, `WriteConcernError` – operation results.
  - `ClientEncryption` – client-side field-level encryption.
- **`src/MongoDB/Exception/`** – exception hierarchy rooted at `Exception` (extends `RuntimeException`), with `CommandException`, `BulkWriteException`, `BulkWriteCommandException`, `ConnectionException`, `EncryptionException`, etc.
- **`src/MongoDB/Monitoring/`** – APM subscriber interfaces and event classes for command monitoring (`CommandStartedEvent`, `CommandSucceededEvent`, `CommandFailedEvent`) and SDAM (`ServerChangedEvent`, `TopologyChangedEvent`, etc.).

### Bundled C Libraries (Submodules)

- `src/libmongoc` – mongo-c-driver (includes libbson)
- `src/libmongocrypt` – client-side encryption library

Build configurations (`config.m4` for Autotools, `config.w32` for Windows) enumerate all bundled source files. When updating a submodule, run `scripts/update-submodule-sources.php` to regenerate these lists and update version checks in both files.

### Test Format (PHPT)

Tests use [PHP's PHPT format](https://qa.php.net/phpt_details.php). Typical structure:

```
--TEST--
Description (JIRA ticket or API description)
--SKIPIF--
<?php require __DIR__ . "/../utils/basic-skipif.inc"; ?>
<?php skip_if_not_live(); ?>
--FILE--
<?php
require_once __DIR__ . "/../utils/basic.inc";
// test code
?>
===DONE===
<?php exit(0); ?>
--EXPECT--
===DONE===
```

`SKIPIF` ordering convention: PHP environment checks → extension build requirements → remote server requirements (most general first) → server-specific requirements → `skip_if_not_clean()`.

Tests are organized under `tests/` by feature area: `bson/`, `cursor/`, `manager/`, `bulk/`, `exception/`, `apm/`, `clientEncryption/`, `bson-corpus/` (spec compliance), etc.
