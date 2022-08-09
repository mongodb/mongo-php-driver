--TEST--
MongoDB\Driver\ClientEncryption::rewrapManyDataKey() when filter matches no keys
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

$orignalKey = $clientEncryption->getKey($keyId);

var_dump($clientEncryption->rewrapManyDataKey(['_id' => 'no-matching-key']));

$modifiedKey = $clientEncryption->getKey($keyId);

var_dump($orignalKey->creationDate == $modifiedKey->creationDate);
var_dump($orignalKey->updateDate == $modifiedKey->updateDate);
var_dump($orignalKey->keyMaterial == $modifiedKey->keyMaterial);

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
object(stdClass)#%d (%d) {
  ["bulkWriteResult"]=>
  NULL
}
bool(true)
bool(true)
bool(true)
===DONE===
