# Releasing

## Merge-up branches

Make sure all open [merge-up pull requests](https://github.com/mongodb/mongo-php-driver/pulls?q=is%3Apr+is%3Aopen+label%3Amerge-up)
containing changes from the release branch have been merged. The release bumps
version numbers across branches, so leaving other changes unmerged makes the
resulting merge conflicts much harder to resolve.

## Transition JIRA issues and version

Find the release version on the
[Manage Versions](https://jira.mongodb.org/plugins/servlet/project-config/PHPC/versions)
page.

All issues associated with the release version should be in the "Closed" state
and have a resolution of "Fixed". Issues with other resolutions (e.g.
"Duplicate", "Works as Designed") should be removed from the release version so
that they do not appear in the release notes.

Check the corresponding ".x" fix version to see if it contains any issues that
are resolved as "Fixed" and should be included in this release version.

Set the version's release date to the current date.

## Trigger the release workflow

Releases are done automatically through a GitHub Action. Visit the corresponding
[Release New Version](https://github.com/mongodb/mongo-php-driver/actions/workflows/release.yml)
workflow page to trigger a new build. Select the correct branch: the default
development branch (`vX.x`) for a new minor release, or the corresponding
maintenance branch (e.g. `v1.18`) for a patch release. Trigger a new run using
the "Run workflow" button. In the following prompt,
enter the version number and the corresponding JIRA version ID for the release.
This version ID can be obtained from a link in the "Version" column on the
[PHPC releases page](https://jira.mongodb.org/projects/PHPC?selectedItem=com.atlassian.jira.jira-projects-plugin%3Arelease-page&status=unreleased).

The automation will create and push the necessary commits and tag, create a
draft release, trigger the packaging builds for the newly created tag, and
publish all required SSDLC assets. The release is created in a draft state and
can be published once the release notes have been updated.

Pre-releases (alpha, beta and RC stability) can be released using the automation
as well. When entering a pre-release version number, make sure to not include a
dash before the stability, e.g. `1.20.0beta1` not `1.20.0-beta1`. PECL versions
do not include a dash before the stability. GitHub Releases for pre-release
versions will be marked as such and will not be marked as "latest" release.
Examples for valid pre-release versions include:
* `1.20.0alpha1`
* `1.20.0beta2`
* `1.20.0RC1` (note the upper-case `RC` suffix)

## Publish release notes

The GitHub release notes are created as a draft, and without any release 
highlights. Fill in release highlights and wait for the entire packaging
workflow to complete. Once all release assets have been uploaded, you can
publish the release. Note that since releases are immutable, publishing release
notes before all packages have been added will result in a broken release.

Once the release is published, mark the version as released on the
[Manage Versions](https://jira.mongodb.org/plugins/servlet/project-config/PHPC/versions)
page.

## Upload package to PECL

Once the packaging workflow has finished creating the PECL package, it will be
published as a release asset in the draft release. This package may be published
via the [Release Upload](https://pecl.php.net/release-upload.php) form. You will
have one chance to confirm the package information after uploading.
