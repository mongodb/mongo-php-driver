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
./configure --enable-developer-flags > /dev/null && \
make clean > /dev/null && make all > /dev/null && make install
```

## Testing

The test suite expects to find `PHONGO-SERVERS.json` in the system temp
directory (e.g. `/tmp`). This file should contain a JSON object with MongoDB
URIs and resemble the following:

```
{
    "STANDALONE": "mongodb:\/\/192.168.112.10:2000",
    "STANDALONE_30": "mongodb:\/\/192.168.112.10:2700",
    "STANDALONE_SSL": "mongodb:\/\/192.168.112.10:2100",
    "STANDALONE_AUTH": "mongodb:\/\/root:toor@192.168.112.10:2200\/?authSource=admin",
    "STANDALONE_X509": "mongodb:\/\/C=US,ST=New York,L=New York City,O=MongoDB,OU=KernelUser,CN=client@192.168.112.10:2300\/?authSource=$external&authMechanism=MONGODB-X509",
    "STANDALONE_PLAIN": "mongodb:\/\/root:toor@192.168.112.10:2400\/?authSource=admin",
    "REPLICASET": "mongodb:\/\/192.168.112.10:3000,192.168.112.10:3001,192.168.112.10:3002\/?replicaSet=REPLICASET",
    "REPLICASET_30": "mongodb:\/\/192.168.112.10:3100,192.168.112.10:3101,192.168.112.10:3102\/?replicaSet=REPLICASET_30",
    "REPLICASET_36": "mongodb:\/\/192.168.112.10:3200,192.168.112.10:3201,192.168.112.10:3202\/?replicaSet=REPLICASET_36"
}
```

The location of this `PHONGO-SERVERS.json` file can be configured by exporting a
`PHONGO_SERVERS` environment variable with the absolute path to the JSON
configuration file.

Our test suite also includes scripts to configure the necessary test environment
with [Vagrant](https://www.vagrantup.com/) and
[Mongo Orchestration](https://github.com/10gen/mongo-orchestration). This is the
preferred way of creating `PHONGO-SERVERS.json` and running the test suite:

```
$ make vm             # Starts the test VMs with Vagrant
$ make test-bootstrap # Starts the mongod servers within the test VM
```

The `test-bootstrap` make target also generates the required
`PHONGO-SERVERS.json` file. The `test` make target may be used to execute the
test suite:

```
$ make test # Executes the test suite against the VMs
```

### Mongo Orchestration

Some tests interact directly with Mongo Orchestration to start their own servers
for cluster testing (e.g. replica set failovers). These tests depend on
`MONGODB_ORCHESTRATION_HOST` and `MONGODB_ORCHESTRATION_PORT` environment
variables.

By default, these tests will use the Mongo Orchestration instance within the
test VM.

### Restarting Mongo Orchestration

If something goes awry in the test VM, it may be helpful to start the VM and
Mongo Orchestration with the following script:

```
#!/bin/sh

rm -f /tmp/PHONGO-SERVERS.json
vagrant reload mo
vagrant ssh mo -c 'sudo rm /home/vagrant/server.pid'
vagrant ssh mo -c 'sudo mongo-orchestration -f mongo-orchestration-config.json -b 192.168.112.10 --enable-majority-read-concern start'
make test-bootstrap
```

## Releasing

The follow steps outline the release process for a maintenance branch (e.g.
releasing the `vX.Y` branch as X.Y.Z).

### Ensure PHP version compatibility

Ensure that the extension compiles on PHP 5.5 through the latest PHP 7.x
release. Be sure to test both ZTS and non-ZTS builds for PHP 5.x.

### Ensure Windows compatibility

PECL will create Windows DLLs for new releases; however, you must ensure that
the extension successfully builds on Windows before releasing. Note that PHP 5.5
and 5.6 require VS2012, while PHP 7.x requires VS2015.

Given the following assumptions:

 * Build directory is `C:\php-sdk\`
 * Compiling for PHP 5.6 (VS2012 x86 Native Tools Command Prompt is running)
 * Extension branch checked out in `C:\php-sdk\phpdev\vc11\x86\pecl\mongodb`

The build process will resemble:

```
cd c:\php-sdk\
bin\phpsdk_setvars.bat

cd C:\php-sdk\phpdev\vc11\x86\php-5.6.12-src
nmake clean
buildconf --force
configure --disable-all --with-openssl --enable-cli --enable-json --enable-mongodb=shared
nmake
```

If the extension was successfully compiled, a `php_mongodb.dll` file should be
generated in the build directory (e.g. `Release_TS`). You should then verify
that the extension loads and executes properly:

```
cd Release_TS
php.exe -d extension=./php_mongodb.dll -m
php.exe -d extension=./php_mongodb.dll -r "var_dump(new MongoDB\Driver\Manager);"
```

See the [internals wiki](https://wiki.php.net/internals/windows/stepbystepbuild)
for more information.

### Transition JIRA issues and version

All issues associated with the release version should be in the "Closed" state
and have a resolution of "Fixed". Issues with other resolutions (e.g.
"Duplicate", "Works as Designed") should be removed from the release version so
that they do not appear in the release notes.

Check the corresponding ".x" fix version to see if it contains any issues that
are resolved as "Fixed" and should be included in this release version.

Update the version's release date and status from the
[Manage Versions](https://jira.mongodb.org/plugins/servlet/project-config/PHPC/versions)
page.

### Update version info

The PHP driver uses [semantic versioning](http://semver.org/). Do not break
backwards compatibility in a non-major release or your users will kill you.

Before proceeding, ensure that the `master` branch is up-to-date with all code
changes in this maintenance branch. This is important because we will later
merge the ensuing release commits up to master with `--strategy=ours`, which
will ignore changes from the merged commits.

Update the version and stability constants in `php_phongo.h`. This should entail
removing the version's "-dev" suffix and changing the stability to "stable":

```
#define PHP_MONGODB_VERSION "1.1.8-dev"
#define PHP_MONGODB_STABILITY "devel"
```

The above would be changed to:

```
#define PHP_MONGODB_VERSION "1.1.8"
#define PHP_MONGODB_STABILITY "stable"
```

The Makefile targets for creating the PECL package depend on these constants, so
you must rebuild the extension after updating `php_phongo.h`.

> **Note:** If this is an alpha or beta release, the version string should
> include the X.Y.Z version followed by the stability and an increment. For
> instance, the first beta release in the 1.4.0 series would be "1.4.0beta1".
> Alpha and beta releases use "alpha" and "beta" stability strings,
> respectively. Release candidates (e.g. "1.4.0RC1") also use "beta" stability.
> See [Documenting release stability and API stability](https://pear.php.net/manual/en/guide.developers.package2.stability.php)
> for more information.

### Publish PECL package

Create the PECL package description file with `make package.xml`. This creates
a `package.xml` file from a template. Version, author, and file information will
be filled in, but release notes must be copied manually from JIRA.

After copying release notes, use `make package` to create the package file (e.g.
`mongodb-X.Y.Z.tgz`) and ensure that it can be successfully installed:

```
$ pecl install -f mongodb-X.Y.Z.tgz
```

The PECL package may be published via the
[Release Upload](https://pecl.php.net/release-upload.php) form. You will have
one chance to confirm the package information after uploading.

### Commit version update and release notes

Commit the modified `php_phongo.h` file as "Package X.Y.Z"

```
$ git add php_phongo.h
$ git commit -m "Package X.Y.Z"
```

### Tag release

The previous commit will be the target for our release tag:

```
$ git tag -a -m "Release X.Y.Z" X.Y.Z
```

### Update version info back to dev

After tagging, the version and stability constants in `php_phongo.h` should be
updated back to development status.

```
#define PHP_MONGODB_VERSION "1.1.8"
#define PHP_MONGODB_STABILITY "stable"
```

The above would be changed to:

```
#define PHP_MONGODB_VERSION "1.1.9-dev"
#define PHP_MONGODB_STABILITY "devel"
```

Commit this change:

```
$ git commit -m "Back to -dev" php_phongo.h
```

> **Note:** If this is an alpha or beta release, the version string revert back
> to its pre-release value without incrementing the patch version. For example,
> the constants may have indicated version "1.4.0-dev" and stability "devel"
> before being changed to "1.4.0beta1" and "beta" for a beta release,
> respectively. After the release, the constants should revert back to
> "1.4.0-dev" and "devel".

### Push commits and tags

```
$ git push
$ git push --tags
```

### Merge the maintenance branch up to master

```
$ git checkout master
$ git merge vX.Y --strategy=ours
$ git push
```

The `--strategy=ours` option ensures that all changes from the merged commits
will be ignored.

### Publish release notes

The following template should be used for creating GitHub release notes via
[this form](https://github.com/mongodb/mongo-php-driver/releases/new). The PECL
package may also be attached to the release notes.

```
The PHP team is happy to announce that version X.Y.Z of our new [mongodb](http://pecl.php.net/package/mongodb) PHP extension is now available on PECL.

**Release Highlights**

<one or more paragraphs describing important changes in this release>

A complete list of resolved issues in this release may be found at:
$JIRA_URL

**Documentation**

Documentation is available on PHP.net:
http://php.net/set.mongodb

**Feedback**

We would appreciate any feedback you might have on the project:
https://jira.mongodb.org/secure/CreateIssue.jspa?pid=12484&issuetype=6

**Installation**

You can either download and install the source manually, or you can install the extension with:

    pecl install mongodb

or update with:

    pecl upgrade mongodb

Windows binaries are available on PECL:
http://pecl.php.net/package/mongodb
```

> **Note:** If this is an alpha or beta release, the installation examples
> should append the stability to the package name (e.g. "mongodb-beta").

The URL for the list of resolved JIRA issues will need to be updated with each
release. You may obtain the list from
[this form](https://jira.mongodb.org/secure/ReleaseNote.jspa?projectId=12484).

If commits from community contributors were included in this release, append the
following section:

```
**Thanks**

Thanks for our community contributors for X.Y.Z:

 * [$CONTRIBUTOR_NAME](https://github.com/$GITHUB_USERNAME)
```

Release announcements should also be sent to the `mongodb-user@googlegroups.com`
and `mongodb-announce@googlegroups.com` mailing lists.

Consider announcing each release on Twitter. Significant releases should also be
announced via [@MongoDB](http://twitter.com/mongodb) as well.
