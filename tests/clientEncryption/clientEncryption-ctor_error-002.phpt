--TEST--
MongoDB\Driver\ClientEncryption::__construct() with invalid option types
--SKIPIF--
<?php require __DIR__ . "/../utils/basic-skipif.inc"; ?>
<?php skip_if_not_libmongocrypt(); ?>
--FILE--
<?php

require_once __DIR__ . '/../utils/basic.inc';

/* phongo_clientencryption_opts_from_zval always requires a keyVaultClient
 * option when constructing ClientEncryption directly, so this will be used to
 * test other options. */
$baseOptions = ['keyVaultClient' => create_test_manager()];

$tests = [
    [],
    ['keyVaultClient' => 'not_an_array_or_object'],
    [
        'keyVaultNamespace' => 'not_a_namespace',
        // keyVaultNamespace requires a valid kmsProviders option
        'kmsProviders' => ['local' => ['key' => new MongoDB\BSON\Binary(CSFLE_LOCAL_KEY, 0)]],
    ] + $baseOptions,
    ['kmsProviders' => 'not_an_array_or_object'] + $baseOptions,
    ['tlsOptions' => 'not_an_array_or_object'] + $baseOptions,
];

foreach ($tests as $test) {
    echo throws(function () use ($test) {
        new MongoDB\Driver\ClientEncryption($test);
    }, MongoDB\Driver\Exception\InvalidArgumentException::class), "\n\n";
}

?>
===DONE===
<?php exit(0); ?>
--EXPECT--
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
The "keyVaultClient" option is required when constructing a ClientEncryption object directly

OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Expected "keyVaultClient" option to be MongoDB\Driver\Manager, string given

OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Expected "keyVaultNamespace" option to contain a full collection namespace

OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Expected "kmsProviders" option to be an array or object, string given

OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Expected "tlsOptions" option to be an array or object, string given

===DONE===
