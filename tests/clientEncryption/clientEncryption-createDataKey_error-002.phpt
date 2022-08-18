--TEST--
MongoDB\Driver\ClientEncryption::createDataKey() with invalid keyMaterial option
--SKIPIF--
<?php require __DIR__ . "/../utils/basic-skipif.inc"; ?>
<?php skip_if_not_libmongocrypt(); ?>
<?php skip_if_not_live(); ?>
--FILE--
<?php

require_once __DIR__ . "/../utils/basic.inc";

$tests = [
    ['keyMaterial' => 0],
    ['keyMaterial' => new stdClass],
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
--EXPECTF--
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Expected "keyMaterial" option to be MongoDB\BSON\Binary, %r(int|integer)%r given
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Expected "keyMaterial" option to be MongoDB\BSON\Binary, stdClass given
===DONE===
