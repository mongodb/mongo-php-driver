--TEST--
MongoDB\Driver\ClientEncryption::getKeys() with empty key vault
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

var_dump(iterator_count($clientEncryption->getKeys()));

?>
===DONE===
<?php exit(0); ?>
--EXPECT--
int(0)
===DONE===
