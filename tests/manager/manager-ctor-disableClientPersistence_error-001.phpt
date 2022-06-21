--TEST--
MongoDB\Driver\Manager and keyVaultClient must have same disableClientPersistence option
--SKIPIF--
<?php require __DIR__ . "/../utils/basic-skipif.inc"; ?>
<?php skip_if_not_libmongocrypt(); ?>
--FILE--
<?php
require_once __DIR__ . "/../utils/basic.inc";

echo throws(function() {
    create_test_manager(null, [], [
        'autoEncryption' => [
            'keyVaultClient' => create_test_manager(null),
            'keyVaultNamespace' => CSFLE_KEY_VAULT_NS,
            'kmsProviders' => ['local' => ['key' => new MongoDB\BSON\Binary(CSFLE_LOCAL_KEY, 0)]],
        ],
        'disableClientPersistence' => true,
    ]);
}, MongoDB\Driver\Exception\InvalidArgumentException::class), "\n";

echo throws(function() {
    create_test_manager(null, [], [
        'autoEncryption' => [
            'keyVaultClient' => create_test_manager(null, [], ['disableClientPersistence' => true]),
            'keyVaultNamespace' => CSFLE_KEY_VAULT_NS,
            'kmsProviders' => ['local' => ['key' => new MongoDB\BSON\Binary(CSFLE_LOCAL_KEY, 0)]],
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
