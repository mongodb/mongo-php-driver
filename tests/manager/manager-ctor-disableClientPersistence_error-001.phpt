--TEST--
MongoDB\Driver\Manager and keyVaultClient must have same disableClientPersistence option
--SKIPIF--
<?php require __DIR__ . "/../utils/basic-skipif.inc"; ?>
<?php skip_if_not_libmongocrypt(); ?>
--FILE--
<?php
require_once __DIR__ . "/../utils/basic.inc";

echo throws(function() {
    new MongoDB\Driver\Manager(null, [], [
        'autoEncryption' => [
            'keyVaultClient' => new MongoDB\Driver\Manager(null),
            'keyVaultNamespace' => 'default.keys',
            'kmsProviders' => ['local' => ['key' => new MongoDB\BSON\Binary(str_repeat('0', 96), 0)]],
        ],
        'disableClientPersistence' => true,
    ]);
}, MongoDB\Driver\Exception\InvalidArgumentException::class), "\n";

echo throws(function() {
    new MongoDB\Driver\Manager(null, [], [
        'autoEncryption' => [
            'keyVaultClient' => new MongoDB\Driver\Manager(null, [], ['disableClientPersistence' => true]),
            'keyVaultNamespace' => 'default.keys',
            'kmsProviders' => ['local' => ['key' => new MongoDB\BSON\Binary(str_repeat('0', 96), 0)]],
        ]
    ]);
}, MongoDB\Driver\Exception\InvalidArgumentException::class), "\n";

?>
===DONE===
<?php exit(0); ?>
--EXPECT--
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
The "disableClientPersistence" option for a Manager and its "keyVaultClient" must be the same
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
The "disableClientPersistence" option for a Manager and its "keyVaultClient" must be the same
===DONE===