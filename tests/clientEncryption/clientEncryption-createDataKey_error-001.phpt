--TEST--
MongoDB\Driver\ClientEncryption::createDataKey() with invalid keyAltNames option
--SKIPIF--
<?php require __DIR__ . "/../utils/basic-skipif.inc"; ?>
<?php skip_if_not_libmongocrypt(); ?>
<?php skip_if_not_live(); ?>
--FILE--
<?php

require_once __DIR__ . "/../utils/basic.inc";

$tests = [
    ['keyAltNames' => 'foo'],
    ['keyAltNames' => [0 => []]],
    ['keyAltNames' => ['foo' => []]],
];

$manager = create_test_manager();
$clientEncryption = $manager->createClientEncryption([
    'keyVaultNamespace' => CSFLE_KEY_VAULT_NS,
    'kmsProviders' => ['local' => ['key' => new MongoDB\BSON\Binary(CSFLE_LOCAL_KEY, 0)]],
]);

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
