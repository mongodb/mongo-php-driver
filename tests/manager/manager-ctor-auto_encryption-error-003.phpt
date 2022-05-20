--TEST--
MongoDB\Driver\Manager::__construct(): invalid option types
--SKIPIF--
<?php require __DIR__ . "/../utils/basic-skipif.inc"; ?>
<?php skip_if_not_libmongocrypt(); ?>
--FILE--
<?php

require_once __DIR__ . '/../utils/basic.inc';

$tests = [
    'not_an_array',
    ['encryptedFieldsMap' => 'not_an_array_or_object'],
    ['keyVaultClient' => 'not_an_array_or_object'],
    [
        'keyVaultNamespace' => 'not_a_namespace',
        // keyVaultNamespace requires a valid kmsProviders option
        'kmsProviders' => ['local' => ['key' => new MongoDB\BSON\Binary('', 0)]],
    ],
    ['kmsProviders' => 'not_an_array_or_object'],
    ['schemaMap' => 'not_an_array_or_object'],
    ['tlsOptions' => 'not_an_array_or_object'],
    ['extraOptions' => 'not_an_array_or_object'],
];

foreach ($tests as $test) {
    echo throws(function() use ($test) {
        $manager = create_test_manager(null, [], ['autoEncryption' => $test]);
    }, MongoDB\Driver\Exception\InvalidArgumentException::class), "\n\n";
}

?>
===DONE===
<?php exit(0); ?>
--EXPECT--
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Expected "autoEncryption" driver option to be array, string given

OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Expected "encryptedFieldsMap" encryption option to be an array or object

OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Expected "keyVaultClient" encryption option to be MongoDB\Driver\Manager, string given

OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Expected "keyVaultNamespace" encryption option to contain a full collection name

OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Expected "kmsProviders" encryption option to be an array or object

OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Expected "schemaMap" encryption option to be an array or object

OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Expected "tlsOptions" encryption option to be an array or object

OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Expected "extraOptions" encryption option to be an array or object

===DONE===
