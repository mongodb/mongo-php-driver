--TEST--
MongoDB\Driver\ClientEncryption::createDataKey() masterKey option invalid type
--SKIPIF--
<?php require __DIR__ . "/../utils/basic-skipif.inc"; ?>
<?php skip_if_not_libmongocrypt(); ?>
<?php skip_if_not_live(); ?>
--FILE--
<?php

require_once __DIR__ . "/../utils/basic.inc";

$manager = create_test_manager();
$clientEncryption = $manager->createClientEncryption([
    'keyVaultNamespace' => CSFLE_KEY_VAULT_NS,
    'kmsProviders' => ['local' => ['key' => new MongoDB\BSON\Binary(CSFLE_LOCAL_KEY, 0)]],
]);

echo throws(function () use ($clientEncryption) {
    $clientEncryption->createDataKey('local', ['masterKey' => 'not-array-or-object']);
}, MongoDB\Driver\Exception\InvalidArgumentException::class), "\n";

?>
===DONE===
<?php exit(0); ?>
--EXPECT--
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Expected "masterKey" option to be array or object, string given
===DONE===
