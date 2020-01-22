--TEST--
MongoDB\Driver\ClientEncryption::createDataKey()
--SKIPIF--
<?php require __DIR__ . "/../utils/basic-skipif.inc"; ?>
<?php skip_if_not_libmongocrypt(); ?>
<?php skip_if_not_live(); ?>
--FILE--
<?php
require_once __DIR__ . "/../utils/basic.inc";

$key = base64_decode('Mng0NCt4ZHVUYUJCa1kxNkVyNUR1QURhZ2h2UzR2d2RrZzh0cFBwM3R6NmdWMDFBMUN3YkQ5aXRRMkhGRGdQV09wOGVNYUMxT2k3NjZKelhaQmRCZGJkTXVyZG9uSjFk');

$manager = new MongoDB\Driver\Manager(URI);
$clientEncryption = $manager->createClientEncryption(['keyVaultNamespace' => 'default.keys', 'kmsProviders' => ['local' => ['key' => new MongoDB\BSON\Binary($key, 0)]]]);

var_dump($clientEncryption->createDataKey('local'));

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
object(MongoDB\BSON\Binary)#%d (%d) {
  ["data"]=>
  string(16) "%a"
  ["type"]=>
  int(4)
}
===DONE===
