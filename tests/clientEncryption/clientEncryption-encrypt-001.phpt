--TEST--
MongoDB\Driver\ClientEncryption::encrypt()
--SKIPIF--
<?php require __DIR__ . "/../utils/basic-skipif.inc"; ?>
<?php skip_if_not_libmongocrypt(); ?>
<?php skip_if_not_live(); ?>
<?php skip_if_server_version('<', '4.2'); ?>
<?php skip_if_not_server_storage_engine('wiredTiger'); ?>
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

var_dump($clientEncryption->encrypt('top-secret', ['keyId' => $keyId, 'algorithm' => MongoDB\Driver\ClientEncryption::AEAD_AES_256_CBC_HMAC_SHA_512_DETERMINISTIC]));

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
object(MongoDB\BSON\Binary)#%d (%d) {
  ["data"]=>
  string(82) "%a"
  ["type"]=>
  int(6)
}
===DONE===
