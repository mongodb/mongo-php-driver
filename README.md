# PHP MongoDB - PHongo
[![Coverage Status](https://coveralls.io/repos/bjori/phongo/badge.png?branch=dry-run)](https://coveralls.io/r/bjori/phongo?branch=dry-run)
[![Build Status](https://travis-ci.org/bjori/phongo.svg?branch=dry-run)](https://travis-ci.org/bjori/phongo)


This repo contains design documentation, examples, and (wip) proof of concept implementation,
of a new MongoDB driver for PHP, Phongo.


The purpose of this driver is to be exceptionally thin glue, implementing only performance
critical aspects of a MongoDB driver, and the fundemental pieces that are required to
build a fully functional MongoDB driver.

In the long run, user-land packages will then be provided ontop of this driver which will
implement all the pretty interfaces (including BC layer for pecl/mongo, and new fluent
interface), management utilities (for creating rock solid Admin Utilities, Cluster
Managers), and other interesting libraries.

The hope is to encourage community contribution and involvment to the various components
along with keeping the overall design fast and powerful.


## Examples:
- [Models](docs/examples/model.php) (used for example data in other examples too)
- [Query](docs/examples/query.php)
- [Insert](docs/examples/insert.php)
- Provide your own [BSON objects](docs/examples/changing-types.php)


## API:

The API is developed using normal PHP userland syntax:

- [Connection Manager](docs/api/MongoDB/Manager.php)
- [ReadPreference](docs/api/MongoDB/ReadPreference.php)
- [WriteBatch](docs/api/MongoDB/WriteBatch.php)
- [WriteResult](docs/api/MongoDB/WriteResult.php)
- [Query](docs/api/MongoDB/Query.php)
- [QueryResult](docs/api/MongoDB/QueryResult.php)
- [Command](docs/api/MongoDB/Command.php)
- [CommandResult](docs/api/MongoDB/CommandResult.php)

The implementation of these objects then use CIMPL (Simple) and CEF (Chef) inline code
snippets which are then included in the generated C code.

The goal is this PHP implementation can serve as the canonical implementation of the
driver that can be used to generate extensions for other PHP implementations (i.e. HHVM).


## Installation
This driver should not be explicitly installed.
Projects should pick one (or more) userland libraries/bindings that are written ontop of
this driver, and install those using composer.
Those libraries/bindings then in turn depends on this driver.




---

This is clearly not meant for the public consumption at the moment and is all highly
experimental and not working.

