# Evergreen Build System Architecture

To avoid unnecessary tasks from being started, this project uses two separate build steps. The first step compiles the
PHP extension on the requested operating system and PHP version, caching the result. The second step runs tests on
various platforms, re-using the cached build artifacts from the first step.

## Build Step 1: Compile PHP Extension

The extension is built for all supported PHP versions on all supported platforms. This build step is run for every
commit pushed to the development branch, as well as for stable branches and pull requests to these branches. The build
step includes a smoke test that attempts to load the compiled extension to catch errors that occur during extension
initialisation. If a build step fails, test tasks are skipped for that platform and PHP version.

### Build tasks

Build tasks are generated automatically and included in the main config file. The `build-variants.yml` file contains
the list of supported platforms that the extension is built for. `build-task-groups.yml` defines the task groups that
contain the build tasks. Note there is a separate task that skips PHP 7.4 and 8.0, as these versions do not support
OpenSSL 3 (currently used on RHEL 9+ and Ubuntu 22.04+).

## Build Step 2: Run Tests

Tests are not offered for all platforms, and only select tasks are run for commits and pull requests. First, all
combinations of MongoDB topologies (standalone, replica set, sharded cluster, and load balanced) are run for all
supported MongoDB versions. Then, all PHP versions are tested against the latest stable version of MongoDB. Finally,
a last build variant runs tests on PHP 8.2 against the latest stable version of MongoDB using different libmongoc
versions.

## Modifying generated tasks

Most build and test tasks are generated using the `generate-config.php` script and referenced using tags. To modify the
configuration, change the appropriate `_template-<build>.yml` file in the `build` or `test` directory, then run the
generator script to update the files. The generator script will output a list of include statements that can be copied
to the main `config.yml` file to include all generated tasks.

## Adding new tasks

Adding new tasks for a new MongoDB version or PHP version is done by modifying `generate-config.php` and including the
version in the corresponding arrays, then regenerating configuration. Note that obsolete files will not be deleted
automatically, but they won't be in the list of include files printed by the generator script.

## Patch Aliases

Aliases are configured in the main Evergreen configuration file. `github_pr_aliases` defines aliases for tasks to be run
for every pull request.

## Task Tags

Tasks are tagged to allow for a better selection in build variants. The following tags are used for build tasks:
- `build`: All build tasks are tagged with `build`.
- `build-libmongoc`: These build tasks build with different libmongoc versions.
- `php<version>`: These tags allow selection based on PHP version, e.g. `php8.2`.

Test tasks use the following tags:
- `local`: All tasks that run a local MongoDB cluster for testing.
- `<version>`: These tags allow selection based on MongoDB version, e.g. `7.0`.
- `standalone`, `replicaset`, `sharded`: These tags allow selection based on the MongoDB topology.
- `loadbalanced`: Allows for selecting tests using a load balancer
- `ocsp`: Used for all OCSP tasks
- `versioned_api`: Used for tests that use the stable API
- `skip_crypt_shared`: These tasks skip installing the shared library to test with libmongocrypt
- `nodb`: These tasks do not rely on a local database
- `atlas`: These tasks work on a MongoDB Atlas cluster
- `storage-engines`: Tag used for tasks that test the `inmemory` and `mmapv1` storage engines
