--TEST--
MongoDB\Driver\Manager::createClientEncryption()
--SKIPIF--
<?php require __DIR__ . "/../utils/basic-skipif.inc"; ?>
<?php skip_if_not_libmongocrypt(); ?>
--FILE--
<?php

$key = base64_decode('Mng0NCt4ZHVUYUJCa1kxNkVyNUR1QURhZ2h2UzR2d2RrZzh0cFBwM3R6NmdWMDFBMUN3YkQ5aXRRMkhGRGdQV09wOGVNYUMxT2k3NjZKelhaQmRCZGJkTXVyZG9uSjFk');

$manager = new MongoDB\Driver\Manager();
$clientEncryption = $manager->createClientEncryption(['keyVaultNamespace' => 'default.keys', 'kmsProviders' => ['local' => ['key' => new MongoDB\BSON\Binary($key, 0)]]]);

?>
===DONE===
<?php exit(0); ?>
--EXPECT--
===DONE===
