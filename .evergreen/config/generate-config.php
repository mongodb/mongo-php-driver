#!/bin/env php
<?php

$buildPhpVersions = [
    '8.2', '8.1', '8.0', '7.4',
];
$libmongocBuildPhpVersions = ['8.2'];

$localServerVersions = [
    'latest', 'rapid',
    '7.0', '6.0', '5.0',
    '4.4', '4.2', '4.0',
    '3.6',
];
$loadBalancedServerVersions = [
    'latest', 'rapid',
    '7.0', '6.0', '5.0',
];
$ocspServerVersions = [
    'latest', 'rapid',
    '7.0', '6.0', '5.0',
    '4.4',
];
$requireApiServerVersions = [
    'latest', 'rapid',
    '7.0', '6.0', '5.0',
];
$skipCryptSharedServerVersions = [
    'latest', 'rapid',
    '7.0', '6.0',
];

$allFiles = [];

$allFiles[] = generateConfigs('build', 'phpVersion', '_template-build-php.yml', 'build-php-%s', $buildPhpVersions);
$allFiles[] = generateConfigs('build', 'phpVersion', '_template-build-libmongoc.yml', 'build-libmongoc-%s', $libmongocBuildPhpVersions);

$allFiles[] = generateConfigs('test', 'mongodbVersion', '_template-local.yml', 'local-%s', $localServerVersions);
$allFiles[] = generateConfigs('test', 'mongodbVersion', '_template-load-balanced.yml', 'load-balanced-%s', $loadBalancedServerVersions);
$allFiles[] = generateConfigs('test', 'mongodbVersion', '_template-ocsp.yml', 'ocsp-%s', $ocspServerVersions);
$allFiles[] = generateConfigs('test', 'mongodbVersion', '_template-require-api-version.yml', 'require-api-version-%s', $requireApiServerVersions);
$allFiles[] = generateConfigs('test', 'mongodbVersion', '_template-skip-crypt-shared.yml', 'skip-crypt-shared-%s', $skipCryptSharedServerVersions);

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
    $template = file_get_contents(__DIR__ . '/' . $directory . '/' . $templateFile);
    $header = '# This file is generated automatically - please edit the corresponding template file!';

    $files = [];

    foreach ($versions as $version) {
        $filename = sprintf('/%s/' . $outputFormat . '.yml', $directory, $version);
        $files[] = '.evergreen/config' . $filename;

        $replacements = ['%' . $replacementName . '%' => $version];

        file_put_contents(__DIR__ . $filename, $header . "\n" . strtr($template, $replacements));
    }

    return $files;
}

