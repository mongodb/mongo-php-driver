--TEST--
MongoDB\Driver\Manager::__construct(): invalid types in autoEncryption options
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
        'kmsProviders' => ['local' => ['key' => new MongoDB\BSON\Binary(CSFLE_LOCAL_KEY, 0)]],
    ],
    ['kmsProviders' => 'not_an_array_or_object'],
    ['schemaMap' => 'not_an_array_or_object'],
    ['tlsOptions' => 'not_an_array_or_object'],
    ['extraOptions' => 'not_an_array_or_object'],
];

foreach ($tests as $autoEncryptionOptions) {
    echo throws(function() use ($autoEncryptionOptions) {
        create_test_manager(null, [], ['autoEncryption' => $autoEncryptionOptions]);
    }, MongoDB\Driver\Exception\InvalidArgumentException::class), "\n\n";
}

?>
===DONE===
<?php exit(0); ?>
--EXPECT--
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Expected "autoEncryption" driver option to be array, string given

OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Expected "encryptedFieldsMap" autoEncryption option to be an array or object, string given

OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Expected "keyVaultClient" autoEncryption option to be MongoDB\Driver\Manager, string given

OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Expected "keyVaultNamespace" autoEncryption option to contain a full collection namespace

OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Expected "kmsProviders" autoEncryption option to be an array or object, string given

OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Expected "schemaMap" autoEncryption option to be an array or object, string given

OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Expected "tlsOptions" autoEncryption option to be an array or object, string given

OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Expected "extraOptions" autoEncryption option to be an array or object, string given

===DONE===
