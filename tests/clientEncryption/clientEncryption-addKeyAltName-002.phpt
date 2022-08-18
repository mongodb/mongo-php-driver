--TEST--
MongoDB\Driver\ClientEncryption::addKeyAltName() when key does not exist
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

$keyId = new MongoDB\BSON\Binary(random_bytes(16), MongoDB\BSON\Binary::TYPE_UUID);

var_dump($clientEncryption->addKeyAltName($keyId, 'foo'));

?>
===DONE===
<?php exit(0); ?>
--EXPECT--
NULL
===DONE===
