#!/usr/bin/env php
<?php

const VERSION_FILENAME = __DIR__ . '/../phongo_version.h';

function usage()
{
    global $argv;

echo <<<EOT
Usage:
{$argv[0]} <command>

Commands:
    stable: Mark the current version as stable
    patch:  Update to the next patch development version
    minor:  Update to the next minor development version

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

    // TODO: Error handling
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
        'version' => get_version_string_from_components($versionComponents) . '-dev',
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
        'version' => get_version_string_from_components($versionComponents) . '-dev',
        'stability' => 'devel',
        'versionComponents' => $versionComponents,
    ];
}

if ($argc !== 2) {
    usage();
}

$currentVersion = read_release_version(VERSION_FILENAME);

switch ($argv[1] ?? null) {
    case 'stable':
        $newVersion = get_stable_version($currentVersion);
        break;

    case 'patch':
        $newVersion = get_next_patch_version($currentVersion);
        break;

    case 'minor':
        $newVersion = get_next_minor_version($currentVersion);
        break;

    default:
        usage();
}

write_release_version(VERSION_FILENAME, $newVersion);

printf("Updated version number in version file from %s to %s\n", $currentVersion['version'], $newVersion['version']);

