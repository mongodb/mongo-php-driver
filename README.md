# PHP MongoDB - PHongo

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


## API design:
- [Connection Manager/Client](docs/api/MongoDB/Management.php)
- [CRUD](docs/api/MongoDB/CRUD.php)
- [BSON types](docs/api/BSON/types.php)


## Installation
This driver should not be explicitly installed.
Projects should pick one (or more) userland libraries/bindings that are written ontop of
this driver, and install those using composer.
Those libraries/bindings then in turn depends on this driver.




---

This is clearly not meant for the public consumption at the moment and is all highly
experimental and not working.

