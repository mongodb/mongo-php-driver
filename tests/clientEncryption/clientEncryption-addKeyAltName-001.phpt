--TEST--
MongoDB\Driver\ClientEncryption::addKeyAltName()
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

$keyId = $clientEncryption->createDataKey('local');
$keyBeforeUpdate = $clientEncryption->addKeyAltName($keyId, 'foo');
$keyAfterUpdate = $clientEncryption->getKey($keyId);

var_dump($keyBeforeUpdate->_id == $keyId);
var_dump(empty($keyBeforeUpdate->keyAltNames));

var_dump($keyAfterUpdate->_id == $keyId);
var_dump(isset($keyAfterUpdate->keyAltNames) && is_array($keyAfterUpdate->keyAltNames));
var_dump(in_array('foo', $keyAfterUpdate->keyAltNames));

?>
===DONE===
<?php exit(0); ?>
--EXPECT--
bool(true)
bool(true)
bool(true)
bool(true)
bool(true)
===DONE===
