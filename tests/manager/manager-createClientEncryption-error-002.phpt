--TEST--
MongoDB\Driver\Manager::createClientEncryption() with invalid option types
--SKIPIF--
<?php require __DIR__ . "/../utils/basic-skipif.inc"; ?>
<?php skip_if_not_libmongocrypt(); ?>
--FILE--
<?php

require_once __DIR__ . '/../utils/basic.inc';

$tests = [
    ['keyVaultClient' => 'not_an_array_or_object'],
    [
        'keyVaultNamespace' => 'not_a_namespace',
        // keyVaultNamespace requires a valid kmsProviders option
        'kmsProviders' => ['local' => ['key' => new MongoDB\BSON\Binary(CSFLE_LOCAL_KEY, 0)]],
    ],
    ['kmsProviders' => 'not_an_array_or_object'],
    ['tlsOptions' => 'not_an_array_or_object'],
];

foreach ($tests as $test) {
    echo throws(function () use ($test) {
        $manager = create_test_manager();
        $clientEncryption = $manager->createClientEncryption($test);
    }, MongoDB\Driver\Exception\InvalidArgumentException::class), "\n\n";
}

?>
===DONE===
<?php exit(0); ?>
--EXPECT--
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Expected "keyVaultClient" option to be MongoDB\Driver\Manager, string given

OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Expected "keyVaultNamespace" option to contain a full collection namespace

OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Expected "kmsProviders" option to be an array or object, string given

OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Expected "tlsOptions" option to be an array or object, string given

===DONE===
