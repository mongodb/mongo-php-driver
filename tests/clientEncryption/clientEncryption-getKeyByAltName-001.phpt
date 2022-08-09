--TEST--
MongoDB\Driver\ClientEncryption::getKeyByAltName()
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

$keyId = $clientEncryption->createDataKey('local', ['keyAltNames' => ['foo', 'bar']]);

$key = $clientEncryption->getKeyByAltName('foo');
var_dump($key->_id == $keyId);

$key = $clientEncryption->getKeyByAltName('bar');
var_dump($key->_id == $keyId);

?>
===DONE===
<?php exit(0); ?>
--EXPECT--
bool(true)
bool(true)
===DONE===
