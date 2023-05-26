<?php

$cmd = "find %s -maxdepth 1 -name '*.c' -print0 | cut -sz -d / -f %d- | sort -dz | tr '\\000' ' '";

$vars = [
    'PHP_MONGODB_COMMON_SOURCES' => 'src/libmongoc/src/common',
    'PHP_MONGODB_KMS_MESSAGE_SOURCES' => 'src/libmongoc/src/kms-message/src',
    'PHP_MONGODB_BSON_SOURCES' => 'src/libmongoc/src/libbson/src/bson',
    'PHP_MONGODB_JSONSL_SOURCES' => 'src/libmongoc/src/libbson/src/jsonsl',
    'PHP_MONGODB_MONGOC_SOURCES' => 'src/libmongoc/src/libmongoc/src/mongoc',
    'PHP_MONGODB_ZLIB_SOURCES' => 'src/libmongoc/src/zlib-1.2.13',
    'PHP_MONGODB_MONGOCRYPT_SOURCES' => 'src/libmongocrypt/src',
    'PHP_MONGODB_MONGOCRYPT_CRYPTO_SOURCES' => 'src/libmongocrypt/src/crypto',
    'PHP_MONGODB_MONGOCRYPT_OS_POSIX_SOURCES' => 'src/libmongocrypt/src/os_posix',
    'PHP_MONGODB_MONGOCRYPT_OS_WIN_SOURCES' => 'src/libmongocrypt/src/os_win',
    'PHP_MONGODB_MONGOCRYPT_KMS_MESSAGE_SOURCES' => 'src/libmongocrypt/kms-message/src',
];

$patterns = [];
$replacements = [];

foreach ($vars as $var => $path) {
    $cutNth = 2 + substr_count($path, '/');

    $files = trim(shell_exec(sprintf($cmd, $path, $cutNth)));

    $patterns[] = sprintf('/(%s=")([^"]*)(";?)/', $var);
    $replacements[] = '$1' . $files . '$3';
}

$files = [
    realpath(__DIR__ . '/../config.m4') => count($patterns),
    // config.w32 does not use PHP_MONGODB_ZLIB_SOURCES (PHPC-1111)
    realpath(__DIR__ . '/../config.w32') => count($patterns) - 1,
];

foreach ($files as $file => $expectedCount) {
    $replaced = preg_replace($patterns, $replacements, file_get_contents($file), 1, $count);

    if ($count !== $expectedCount) {
        fprintf(STDERR, "Skipping %s: Expected %d replacements but only matched %d\n", basename($file), $expectedCount, $count);
        continue;
    }

    printf("Updated %s\n", basename($file));
    file_put_contents($file, $replaced);
}
