--TEST--
MongoDB\Driver\ClientEncryption::createDataKey() with invalid keyAltNames
--SKIPIF--
<?php require __DIR__ . "/../utils/basic-skipif.inc"; ?>
<?php skip_if_not_libmongocrypt(); ?>
<?php skip_if_not_live(); ?>
--FILE--
<?php
require_once __DIR__ . "/../utils/basic.inc";

$key = base64_decode('Mng0NCt4ZHVUYUJCa1kxNkVyNUR1QURhZ2h2UzR2d2RrZzh0cFBwM3R6NmdWMDFBMUN3YkQ5aXRRMkhGRGdQV09wOGVNYUMxT2k3NjZKelhaQmRCZGJkTXVyZG9uSjFk');

$tests = [
    ['keyAltNames' => 'foo'],
    ['keyAltNames' => [0 => []]],
    ['keyAltNames' => ['foo' => []]],
];

$manager = new MongoDB\Driver\Manager(URI);
$clientEncryption = $manager->createClientEncryption(['keyVaultNamespace' => 'default.keys', 'kmsProviders' => ['local' => ['key' => new MongoDB\BSON\Binary($key, 0)]]]);

foreach ($tests as $opts) {
    echo throws(function () use ($clientEncryption, $opts) {
        $clientEncryption->createDataKey('local', $opts);
    }, MongoDB\Driver\Exception\InvalidArgumentException::class), "\n";
}

?>
===DONE===
<?php exit(0); ?>
--EXPECT--
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Expected keyAltNames to be array, string given
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Expected keyAltName with index "0" to be string, array given
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Expected keyAltName with index "foo" to be string, array given
===DONE===
