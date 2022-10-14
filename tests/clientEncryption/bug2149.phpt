--TEST--
MongoDB\Driver\ClientEncryption::createDataKey() may leak "masterKey" option
--SKIPIF--
<?php require __DIR__ . "/../utils/basic-skipif.inc"; ?>
<?php skip_if_not_libmongocrypt(); ?>
<?php skip_if_not_live(); ?>
<?php skip_if_server_version('<', '4.2'); ?>
<?php skip_if_not_clean(CSFLE_KEY_VAULT_DATABASE_NAME, CSFLE_KEY_VAULT_COLLECTION_NAME);
--FILE--
<?php

require_once __DIR__ . "/../utils/basic.inc";

$manager = create_test_manager();

$clientEncryption = $manager->createClientEncryption([
  'keyVaultNamespace' => CSFLE_KEY_VAULT_NS,
  'kmsProviders' => ['local' => ['key' => new MongoDB\BSON\Binary(CSFLE_LOCAL_KEY, 0)]],
]);

/* The "local" provider does not support a "masterKey" option, but this is
 * sufficient to grow the stack-allocated bson_t and test for a possible leak. */
throws(function () use ($clientEncryption) {
    $clientEncryption->createDataKey('local', ['masterKey' => ['foo' => str_repeat('a', 4096)]]);
}, MongoDB\Driver\Exception\EncryptionException::class);

?>
===DONE===
<?php exit(0); ?>
--EXPECT--
OK: Got MongoDB\Driver\Exception\EncryptionException
===DONE===
