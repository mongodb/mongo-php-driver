# Contributing to the PHP Driver for MongoDB


## Building from VCS (GitHub)

Developers who would like to contribute to the driver will need to build it from VCS
like so:

```
$ git clone https://github.com/mongodb/mongo-php-driver.git phongo
$ cd phongo
$ git submodule update --init
$ phpize
$ ./configure --enable-developer-flags
$ make -j8 all
```

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

## Releasing

The follow steps outline the release process for a maintenance branch (e.g.
releasing the `vX.Y` branch as X.Y.Z).

### Ensure PHP version compatibility

Ensure that the extension compiles on PHP 5.4, 5.5, 5.6, and 7.0. Be sure to
test both ZTS and non-ZTS builds for PHP 5.x.

### Ensure Windows compatibility

PECL will create Windows DLLs for new releases; however, you must ensure that
the extension successfully builds on Windows before releasing. Note that PHP 5.5
and 5.6 require VS2012, while PHP 7 requires VS2015.

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
configure --disable-all --with-openssl --enable-cli --enable-json --enable-mongodb
nmake
```

If the extension was successfully compiled, "mongodb" should be reported by
`Release_TS\php.exe -m`.

See the [internals wiki](https://wiki.php.net/internals/windows/stepbystepbuild)
for more information.

### Transition JIRA issues and version

Update the fix version field for all resolved issues with the corresponding ".x"
fix version.

Update the version's release date and status from the
[Manage Versions](https://jira.mongodb.org/plugins/servlet/project-config/PHPC/versions)
page.

Transition all resolved issues for this version to the closed state. If changing
the issues in bulk, be sure to allow email notifications.

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
#define MONGODB_VERSION_S "1.1.8-dev"
#define MONGODB_STABILITY_S "devel"
```

The above would be changed to:

```
#define MONGODB_VERSION_S "1.1.8"
#define MONGODB_STABILITY_S "stable"
```

The Makefile targets for creating the PECL package depend on these constants, so
you must rebuild the extension after updating `php_phongo.h`.

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
#define MONGODB_VERSION_S "1.1.8"
#define MONGODB_STABILITY_S "stable"
```

The above would be changed to:

```
#define MONGODB_VERSION_S "1.1.9-dev"
#define MONGODB_STABILITY_S "devel"
```

Commit this change:

```
$ git commit -m "Back to -dev" php_phongo.h
```

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
