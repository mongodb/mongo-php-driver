<?php

$cmd = "find %s -maxdepth 1 -name '*.c' -print0 | cut -sz -d / -f %d- | sort -dz | tr '\\000' ' '";

// On macOS, use gcut from the coreutils brew package instead of cut
if (PHP_OS_FAMILY === 'Darwin') {
    $cmd = str_replace('cut', 'gcut', $cmd);
}

$vars = [
    'PHP_MONGODB_COMMON_SOURCES' => 'src/libmongoc/src/common',
    'PHP_MONGODB_KMS_MESSAGE_SOURCES' => 'src/libmongoc/src/kms-message/src',
    'PHP_MONGODB_BSON_SOURCES' => 'src/libmongoc/src/libbson/src/bson',
    'PHP_MONGODB_JSONSL_SOURCES' => 'src/libmongoc/src/libbson/src/jsonsl',
    'PHP_MONGODB_MONGOC_SOURCES' => 'src/libmongoc/src/libmongoc/src/mongoc',
    'PHP_MONGODB_UTF8PROC_SOURCES' => 'src/libmongoc/src/utf8proc-2.8.0',
    'PHP_MONGODB_ZLIB_SOURCES' => 'src/libmongoc/src/zlib-1.2.13',
    'PHP_MONGODB_MONGOCRYPT_SOURCES' => 'src/libmongocrypt/src',
    'PHP_MONGODB_MONGOCRYPT_CRYPTO_SOURCES' => 'src/libmongocrypt/src/crypto',
    'PHP_MONGODB_MONGOCRYPT_OS_POSIX_SOURCES' => 'src/libmongocrypt/src/os_posix',
    'PHP_MONGODB_MONGOCRYPT_OS_WIN_SOURCES' => 'src/libmongocrypt/src/os_win',
    'PHP_MONGODB_MONGOCRYPT_KMS_MESSAGE_SOURCES' => 'src/libmongocrypt/kms-message/src',
    // Note: src/libmongocrypt/src/mlib does not contain source files (as of libmongocrypt 1.3.2)
];

$patterns = [];
$replacements = [];

foreach ($vars as $var => $path) {
    $cutNth = 2 + substr_count($path, '/');

    $files = trim(shell_exec(sprintf($cmd, $path, $cutNth)));

    // Note: utf8proc_data.c is included from utf8proc.c and should not be compiled directly
    if ($var === 'PHP_MONGODB_UTF8PROC_SOURCES') {
        $files = trim(str_replace('utf8proc_data.c', '', $files));
    }

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
