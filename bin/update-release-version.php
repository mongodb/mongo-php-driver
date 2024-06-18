#!/usr/bin/env php
<?php

const VERSION_FILENAME = __DIR__ . '/../phongo_version.h';

function usage()
{
    global $argv;

echo <<<EOT
Usage:
{$argv[0]} <command> [<version>]

Commands:
    release:            Release the given version (requires second argument)
    to-next-dev:        Update to the next version following the current version
    to-stable:          Mark the current version as stable
    to-next-patch-dev:  Update to the next patch development version
    to-next-minor-dev:  Update to the next minor development version
    get-version:        Print the current version number

EOT;

    exit(1);
}

function read_release_version(string $filename): array
{
    if (! is_file($filename)) {
        throw new Exception(sprintf('File not found: "%s"', $filename));
    }

    $contents = file_get_contents($filename);

    $versions = [];

    if (! preg_match('/^#define PHP_MONGODB_VERSION "(.*)"$/m', $contents, $matches)) {
        throw new Exception('Could not match PHP_MONGODB_VERSION');
    }
    $versions['version'] = $matches[1];

    if (! preg_match('/^#define PHP_MONGODB_STABILITY "(.*)"$/m', $contents, $matches)) {
        throw new Exception('Could not match PHP_MONGODB_STABILITY');
    }
    $versions['stability'] = $matches[1];

    if (! preg_match('/^#define PHP_MONGODB_VERSION_DESC (\d+),(\d+),(\d+),(\d+)$/m', $contents, $matches)) {
        throw new Exception('Could not match PHP_MONGODB_VERSION_DESC');
    }
    $versions['versionComponents'] = array_slice($matches, 1);

    return $versions;
}

function parse_release_version(string $version): array
{
    // Regex copied from https://github.com/pear/pear-core/blob/6f4c3a0b134626d238d75a44af01a2f7c4e688d9/PEAR/Common.php#L32
    if (! preg_match('#^(?<major>\d+)\.(?<minor>\d+)\.(?<patch>\d+)(?:(?<stability>(?:alpha|beta))(?<prereleasenum>\d+))?$#', $version, $matches)) {
        throw new Exception(sprintf('Given version "%s" is not in the PEAR version format'));
    }

    return [
        'version' => $version,
        'stability' => $matches['stability'] ?? 'stable',
        'versionComponents' => [
            $matches['major'],
            $matches['minor'],
            $matches['patch'],
            0,
        ],
    ];
}

function write_release_version(string $filename, array $version): void
{
    if (! is_file($filename)) {
        throw new Exception(sprintf('File not found: "%s"', $filename));
    }

    $contents = file_get_contents($filename);

    $patterns = [
        '/^#define PHP_MONGODB_VERSION "(.*)"$/m',
        '/^#define PHP_MONGODB_STABILITY "(.*)"$/m',
        '/^#define PHP_MONGODB_VERSION_DESC (\d+),(\d+),(\d+),(\d+)$/m',
    ];

    $replacements = [
        sprintf('#define PHP_MONGODB_VERSION "%s"', $version['version']),
        sprintf('#define PHP_MONGODB_STABILITY "%s"', $version['stability']),
        sprintf('#define PHP_MONGODB_VERSION_DESC %s,%s,%s,%s', ...$version['versionComponents']),
    ];

    $contents = preg_replace($patterns, $replacements, $contents, -1, $count);
    if ($count !== 3) {
        throw new Exception('Could not properly replace contents in file');
    }

    file_put_contents($filename, $contents);
}

function get_version_string_from_components(array $versionComponents): string
{
    return implode('.', array_slice($versionComponents, 0, 3));
}

function get_stable_version(array $versions): array
{
    if (! preg_match('/^(\d+\.\d+\.\d+)/', $versions['version'], $matches)) {
        throw new Exception(sprintf('Version "%s" is not in the expected format', $versions['version']));
    }

    $version = $matches[1];

    $expectedVersionString = get_version_string_from_components($versions['versionComponents']);

    if ($expectedVersionString !== $version) {
        throw new Exception(sprintf('Version "%s" does not match version from components ("%s")', $version, $expectedVersionString));
    }

    $newVersions = [
        'version' => $version,
        'stability' => 'stable',
        'versionComponents' => $versions['versionComponents'],
    ];

    // Increase build number
    $newVersions['versionComponents'][3]++;

    return $newVersions;
}

function get_next_patch_version(array $versions): array
{
    $versionComponents = $versions['versionComponents'];

    // Increase patch version, set build number to 0
    $versionComponents[2] += 1;
    $versionComponents[3] = 0;

    return [
        'version' => get_version_string_from_components($versionComponents) . 'dev',
        'stability' => 'devel',
        'versionComponents' => $versionComponents,
    ];
}

function get_next_minor_version(array $versions): array
{
    $versionComponents = $versions['versionComponents'];

    // Increase minor version, set patch and build number to 0
    $versionComponents[1] += 1;
    $versionComponents[2] = 0;
    $versionComponents[3] = 0;

    return [
        'version' => get_version_string_from_components($versionComponents) . 'dev',
        'stability' => 'devel',
        'versionComponents' => $versionComponents,
    ];
}

function get_next_release_version(array $versions, string $releaseVersion): array
{
    $releaseVersion = parse_release_version($releaseVersion);

    // When bumping to the specified release version, check if the major, minor, and patch versions match what's currently in the file
    if (array_slice($versions['versionComponents'], 0, 3) !== array_slice($releaseVersion['versionComponents'], 0, 3)) {
        throw new Exception(sprintf('Cannot bump version "%s" to version "%s".', $versions['version'], $releaseVersion['version']));
    }

    // Now, re-use the existing version components to copy over the previous build number used for DLLs
    $releaseVersion['versionComponents'] = $versions['versionComponents'];

    return $releaseVersion;
}

function get_next_dev_version(array $versions): array
{
    $versionComponents = $versions['versionComponents'];

    // We're dealing with a pre-release. The next version is a devel version of the corresponding stable release
    // Examples:
    // 1.19.1snapshot => 1.19.1dev
    // 1.20.0alpha1 => 1.20.0dev
    // 1.20.0beta1 => 1.20.0dev
    if ($versions['stability'] != 'stable') {
        // Increase the build number for unique DLL versions
        $versionComponents[3]++;

        return [
            'version' => get_version_string_from_components($versionComponents) . 'dev',
            'stability' => 'devel',
            'versionComponents' => $versionComponents,
        ];
    }

    // For all other releases, return the next patch version
    return get_next_patch_version($versions);
}

// Allow 2 arguments as the bump-version action always passes a version number, even when not needed
if (! in_array($argc, [2, 3])) {
    usage();
}

$currentVersion = read_release_version(VERSION_FILENAME);

switch ($argv[1] ?? null) {
    case 'get-version':
        echo $currentVersion['version'];

        exit(0);

    case 'release':
        if ($argc !== 3) {
            usage();
        }

        $newVersion = get_next_release_version($currentVersion, $argv[2]);
        break;

    case 'to-next-dev':
        $newVersion = get_next_dev_version($currentVersion);
        break;

    case 'to-stable':
        $newVersion = get_stable_version($currentVersion);
        break;

    case 'to-next-patch-dev':
        $newVersion = get_next_patch_version($currentVersion);
        break;

    case 'to-next-minor-dev':
        $newVersion = get_next_minor_version($currentVersion);
        break;

    default:
        usage();
}

write_release_version(VERSION_FILENAME, $newVersion);

printf("Updated version number in version file from %s to %s\n", $currentVersion['version'], $newVersion['version']);
