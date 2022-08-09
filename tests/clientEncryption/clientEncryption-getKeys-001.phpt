--TEST--
MongoDB\Driver\ClientEncryption::getKeys()
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

$key1Id = $clientEncryption->createDataKey('local');
$key2Id = $clientEncryption->createDataKey('local');

$keyIds = [];

foreach ($clientEncryption->getKeys() as $key) {
    $keyIds[] = $key->_id;
}

var_dump(count($keyIds));
var_dump(in_array($key1Id, $keyIds));
var_dump(in_array($key2Id, $keyIds));

?>
===DONE===
<?php exit(0); ?>
--EXPECT--
int(2)
bool(true)
bool(true)
===DONE===
