#!/bin/env php
<?php

// Supported PHP versions. Add new versions to the beginning of the list
$supportedPhpVersions = [
    '8.2',
    '8.1',
    '8.0',
    '7.4',
];

// Supported MongoDB versions. Add new versions after "rapid"
$supportedMongoDBVersions = [
    'latest',
    'rapid',
    '7.0',
    '6.0',
    '5.0',
    '4.4',
    '4.2',
    '4.0',
    '3.6',
];

// Only test the latest PHP version for libmongoc
$libmongocBuildPhpVersions = [ max($supportedPhpVersions) ];

// Server versions
$localServerVersions = $supportedMongoDBVersions;
$loadBalancedServerVersions = array_filter(
    $supportedMongoDBVersions,
    // Load balanced supports MongoDB 5.0+
    fn (string $version): bool => in_array($version, ['latest', 'rapid']) || version_compare($version, '5.0', '>='),
);
$ocspServerVersions = array_filter(
    $supportedMongoDBVersions,
    // OCSP supports MongoDB 4.4+
    fn (string $version): bool => in_array($version, ['latest', 'rapid']) || version_compare($version, '4.4', '>='),
);
$requireApiServerVersions = array_filter(
    $supportedMongoDBVersions,
    // requireApiVersion supports MongoDB 5.0+
    fn (string $version): bool => in_array($version, ['latest', 'rapid']) || version_compare($version, '5.0', '>='),
);
$skipCryptSharedServerVersions = array_filter(
    $supportedMongoDBVersions,
    // crypt_shared is available starting with MongoDB 6.0
    fn (string $version): bool => in_array($version, ['latest', 'rapid']) || version_compare($version, '6.0', '>='),
);

$allFiles = [];

// Build tasks
$allFiles[] = generateConfigs('build', 'phpVersion', 'build-php.yml', 'build-php-%s', $supportedPhpVersions);
$allFiles[] = generateConfigs('build', 'phpVersion', 'build-libmongoc.yml', 'build-libmongoc-%s', $libmongocBuildPhpVersions);

// Test tasks
$allFiles[] = generateConfigs('test', 'mongodbVersion', 'local.yml', 'local-%s', $localServerVersions);
$allFiles[] = generateConfigs('test', 'mongodbVersion', 'load-balanced.yml', 'load-balanced-%s', $loadBalancedServerVersions);
$allFiles[] = generateConfigs('test', 'mongodbVersion', 'ocsp.yml', 'ocsp-%s', $ocspServerVersions);
$allFiles[] = generateConfigs('test', 'mongodbVersion', 'require-api-version.yml', 'require-api-version-%s', $requireApiServerVersions);
$allFiles[] = generateConfigs('test', 'mongodbVersion', 'skip-crypt-shared.yml', 'skip-crypt-shared-%s', $skipCryptSharedServerVersions);

echo "Generated config. Use the following list to import files:\n";
echo implode("\n", array_map('getImportConfig', array_merge(...$allFiles))) . "\n";

function getImportConfig(string $filename): string
{
    return '- filename: ' . $filename;
}

function generateConfigs(
    string $directory,
    string $replacementName,
    string $templateFile,
    string $outputFormat,
    array $versions,
): array {
    $templateRelativePath = 'templates/' . $directory . '/' . $templateFile;
    $template = file_get_contents(__DIR__ . '/' . $templateRelativePath);
    $header = sprintf(
        '# This file is generated automatically - please edit the "%s" template file instead.',
        $templateRelativePath
    );

    $files = [];

    foreach ($versions as $version) {
        $filename = sprintf('/generated/%s/' . $outputFormat . '.yml', $directory, $version);
        $files[] = '.evergreen/config' . $filename;

        $replacements = ['%' . $replacementName . '%' => $version];

        file_put_contents(__DIR__ . $filename, $header . "\n" . strtr($template, $replacements));
    }

    return $files;
}

