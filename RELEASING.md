# Releasing

## Branch management

When releasing a new minor version of the driver (i.e. `X.Y.0`), create a new
maintenance branch named `vX.Y` from the default branch. Then, update the
version information in the default branch to the next minor release:

```shell
$ ./bin/update-release-version.php to-next-minor-dev
```

Commit and push the resulting changes:

```shell
$ git commit -m "Master is now X.Y-dev" phongo_version.h
$ git push mongodb
```

Before starting the release process, make sure that all open merge-up pull
requests containing changes from the release branch have been merged. Handling
the merge conflicts resulting from the version updates will be much more
difficult if other changes need merging.

## Transition JIRA issues and version

All issues associated with the release version should be in the "Closed" state
and have a resolution of "Fixed". Issues with other resolutions (e.g.
"Duplicate", "Works as Designed") should be removed from the release version so
that they do not appear in the release notes.

Check the corresponding ".x" fix version to see if it contains any issues that
are resolved as "Fixed" and should be included in this release version.

Update the version's release date and status from the
[Manage Versions](https://jira.mongodb.org/plugins/servlet/project-config/PHPC/versions)
page.

## Trigger the release workflow

Releases are done automatically through a GitHub Action. Visit the corresponding
[Release New Version](https://github.com/mongodb/mongo-php-driver/actions/workflows/release.yml)
workflow page to trigger a new build. Select the correct branch (e.g. `v1.18`)
and trigger a new run using the "Run workflow" button. In the following prompt,
enter the version number and the corresponding JIRA version ID for the release.
This version ID can be obtained from a link in the "Version" column on the
[PHPC releases page](https://jira.mongodb.org/projects/PHPC?selectedItem=com.atlassian.jira.jira-projects-plugin%3Arelease-page&status=unreleased).

The automation will then create and push the necessary commits and tag, create a
draft release, and trigger the packaging builds for the newly created tag. The
release is created in a draft state and can be published once the release notes
have been updated.

Alternatively, you may follow the [manual release process](#manual-release-process)
before continuing with the next section.

## Upload package to PECL

Once the packaging workflow has finished creating the PECL package, it will be
published as a build artifact of the package workflow, as well as a release
asset in the draft release. This package may be published via the
[Release Upload](https://pecl.php.net/release-upload.php) form. You will have one chance to confirm the package
information after uploading.

> [!NOTE]
> If downloading the package from the build artifacts, be aware that these are
> always provided as zip files. You'll have to unpack the tgz archive prior to
> uploading it to PECL. This is not the case when downloading the release asset.

## Update compatibility tables in MongoDB docs

For minor releases, create a DOCSP ticket indicating whether there are changes
to MongoDB Server or PHP version compatibility. The [compatibility tables](https://docs.mongodb.com/drivers/driver-compatibility-reference#php-driver-compatibility)
in the MongoDB documentation must be updated to account for new releases. Make
sure to update both MongoDB and Language compatibility tables, as shown in
[this pull request](https://github.com/mongodb/docs-ecosystem/pull/642).

## Handle merge-up pull request

After the release automation pushes changes to the stable branch the release was
created from, the merge automation will create a new pull request to merge these
changes into the next versioned branch. Since version changes always create a
conflict, follow the "Ignoring Changes" section in the pull request to resolve
the conflicts and merge the pull request once the build completes.

## Announce release

Significant release announcements should also be posted in the
[MongoDB Product & Driver Announcements: Driver Releases](https://www.mongodb.com/community/forums/tags/c/announcements/driver-releases/110/php) forum.

## Manual release process

The following steps outline the manual release process. These are preserved
for historical reasons and releases that are currently not supported by the
release automation (e.g. beta releases). These steps are meant to be run instead
of [triggering the release workflow](#trigger-the-release-workflow). The
instructions assume that the steps preceding the release workflow have been
completed successfully.

The command examples below assume that the canonical "mongodb" repository has
the remote name "mongodb". You may need to adjust these commands if you've given
the remote another name (e.g. "upstream"). The "origin" remote name was not used
as it likely refers to your personal fork.

It helps to keep your own fork in sync with the "mongodb" repository (i.e. any
branches and tags on the main repository should also exist in your fork). This
is left as an exercise to the reader.

### Update version info

The PHP driver uses [semantic versioning](http://semver.org/). Do not break
backwards compatibility in a non-major release or your users will kill you.

Before proceeding, ensure that the `master` branch is up-to-date with all code
changes in this maintenance branch. This is important because we will later
merge the ensuing release commits up to master with `--strategy=ours`, which
will ignore changes from the merged commits.

Update the version and stability constants in `phongo_version.h` for the stable
release:

```shell
$ ./bin/update-release-version.php to-stable
```

The Makefile targets for creating the PECL package depend on these constants, so
you must rebuild the extension after updating `phongo_version.h`.

> [!NOTE]
> If this is an alpha or beta release, the version string should include the
> X.Y.Z version followed by the stability and an increment. For instance, the
> first beta release in the 1.4.0 series would be "1.4.0beta1". Alpha and beta
> releases use "alpha" and "beta" stability strings, respectively. Release
> candidates (e.g. "1.4.0RC1") also use "beta" stability. See
> [Documenting release stability and API stability](https://pear.php.net/manual/en/guide.developers.package2.stability.php)
> for more information. For each change to the suffixes of
> `PHP_MONGODB_VERSION`, increment the last digit of
> `PHP_MONGODB_VERSION_DESC`.

### Build PECL package

Create the PECL package description file with `make package.xml`. This creates
a `package.xml` file from a template. Version, author, and file information will
be filled in, but release notes must be copied manually from JIRA.

After copying release notes, use `make package` to create the package file (e.g.
`mongodb-X.Y.Z.tgz`) and ensure that it can be successfully installed:

```
$ pecl install -f mongodb-X.Y.Z.tgz
```

### Update version info

Commit the modified `phongo_version.h` file and push this change:

```
$ git commit -m "Package X.Y.Z" phongo_version.h
$ git push mongodb
```

### Tag the release

Create a tag for the release and push:

```
$ git tag -a -m "Release X.Y.Z" X.Y.Z
$ git push mongodb --tags
```

Pushing the new tag will start the packaging process which provides the PECL
and Windows packages that should be attached to the release.

### Release PECL package

The PECL package may be published via the
[Release Upload](https://pecl.php.net/release-upload.php) form. You will have
one chance to confirm the package information after uploading.

### Update version info back to dev

After tagging, the version and stability constants in `phongo_version.h` should
be updated back to development status:

```shell
$ ./bin/update-release-version.php to-next-patch-dev
```

Commit and push this change:

```
$ git commit -m "Back to -dev" phongo_version.h
$ git push mongodb
```

> [!NOTE]
> If this is an alpha, beta, or RC release, the version string should increment
> the stability sequence instead of the patch version. For example, if the
> constants were originally "1.4.0-dev" and "devel" and then changed to
> "1.4.0beta1" and "beta" for the first beta release, this step would see them
> ultimately changed to "1.4.0beta2-dev" and "devel".

### Publish release notes

The following template should be used for creating GitHub release notes via
[this form](https://github.com/mongodb/mongo-php-driver/releases/new). The PECL
package may also be attached to the release notes.

```markdown
The PHP team is happy to announce that version X.Y.Z of the [mongodb](https://pecl.php.net/package/mongodb) PHP extension is now available on PECL.

**Release Highlights**

<one or more paragraphs describing important changes in this release>

A complete list of resolved issues in this release may be found in [JIRA]($JIRA_URL).

**Documentation**

Documentation is available on [PHP.net](https://php.net/set.mongodb).

**Installation**

You can either download and install the source manually, or you can install the extension with:

    pecl install mongodb-X.Y.Z

or update with:

    pecl upgrade mongodb-X.Y.Z

Windows binaries are attached to the GitHub release notes.
```

> [!NOTE]
> If this is an alpha or beta release, the installation examples should refer to
> the exact version (e.g. "mongodb-1.8.0beta2"). This is necessary because PECL
> prioritizes recent, stable releases over any stability preferences
> (e.g. "mongodb-beta").

The URL for the list of resolved JIRA issues will need to be updated with each
release. You may obtain the list from
[this form](https://jira.mongodb.org/secure/ReleaseNote.jspa?projectId=12484).

If commits from community contributors were included in this release, append the
following section:

```markdown
**Thanks**

Thanks for our community contributors for X.Y.Z:

* [$CONTRIBUTOR_NAME](https://github.com/$GITHUB_USERNAME)
```
