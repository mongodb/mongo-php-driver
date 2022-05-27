--TEST--
MongoDB\Driver\ClientEncryption::__construct()
--SKIPIF--
<?php require __DIR__ . "/../utils/basic-skipif.inc"; ?>
<?php skip_if_not_libmongocrypt(); ?>
--FILE--
<?php

$key = base64_decode('Mng0NCt4ZHVUYUJCa1kxNkVyNUR1QURhZ2h2UzR2d2RrZzh0cFBwM3R6NmdWMDFBMUN3YkQ5aXRRMkhGRGdQV09wOGVNYUMxT2k3NjZKelhaQmRCZGJkTXVyZG9uSjFk');

$clientEncryption = new MongoDB\Driver\ClientEncryption([
    'keyVaultClient' => new MongoDB\Driver\Manager(),
    'keyVaultNamespace' => 'default.keys',
    'kmsProviders' => ['local' => ['key' => new MongoDB\BSON\Binary($key, 0)]]
]);

var_dump($clientEncryption);

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
object(MongoDB\Driver\ClientEncryption)#%d (%d) {
}
===DONE===
