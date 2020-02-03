--TEST--
MongoDB\Driver\ClientEncryption::encrypt()
--SKIPIF--
<?php require __DIR__ . "/../utils/basic-skipif.inc"; ?>
<?php skip_if_not_libmongocrypt(); ?>
<?php skip_if_not_live(); ?>
<?php skip_if_server_version('<', '3.6'); ?>
<?php skip_if_not_server_storage_engine('wiredTiger'); ?>
--FILE--
<?php
require_once __DIR__ . "/../utils/basic.inc";

$key = base64_decode('Mng0NCt4ZHVUYUJCa1kxNkVyNUR1QURhZ2h2UzR2d2RrZzh0cFBwM3R6NmdWMDFBMUN3YkQ5aXRRMkhGRGdQV09wOGVNYUMxT2k3NjZKelhaQmRCZGJkTXVyZG9uSjFk');

$manager = new MongoDB\Driver\Manager(URI);
$clientEncryption = $manager->createClientEncryption(['keyVaultNamespace' => 'default.keys', 'kmsProviders' => ['local' => ['key' => new MongoDB\BSON\Binary($key, 0)]]]);

$key = $clientEncryption->createDataKey('local');

var_dump($clientEncryption->encrypt('top-secret', ['keyId' => $key, 'algorithm' => MongoDB\Driver\ClientEncryption::AEAD_AES_256_CBC_HMAC_SHA_512_DETERMINISTIC]));

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
