--TEST--
MongoDB\Driver\ClientEncryption::encrypt() stringOpts integer validation errors
--SKIPIF--
<?php require __DIR__ . "/../utils/basic-skipif.inc"; ?>
<?php skip_if_not_libmongocrypt(); ?>
--FILE--
<?php

require_once __DIR__ . "/../utils/basic.inc";

$manager = create_test_manager();

$clientEncryption = $manager->createClientEncryption([
    'keyVaultNamespace' => CSFLE_KEY_VAULT_NS,
    'kmsProviders' => ['local' => ['key' => new MongoDB\BSON\Binary(CSFLE_LOCAL_KEY, 0)]],
]);

$keyId = new MongoDB\BSON\Binary(str_repeat("\x00", 16), MongoDB\BSON\Binary::TYPE_UUID);

$baseOpts = [
    'keyId' => $keyId,
    'algorithm' => 'String',
    'queryType' => 'substringPreview',
    'contentionFactor' => 0,
];

// prefix.strMaxQueryLength out of range
echo throws(function() use ($clientEncryption, $baseOpts) {
    $clientEncryption->encrypt('test', $baseOpts + ['stringOpts' => [
        'prefix' => ['strMaxQueryLength' => -1],
    ]]);
}, MongoDB\Driver\Exception\InvalidArgumentException::class), "\n";

// prefix.strMinQueryLength out of range
echo throws(function() use ($clientEncryption, $baseOpts) {
    $clientEncryption->encrypt('test', $baseOpts + ['stringOpts' => [
        'prefix' => ['strMinQueryLength' => PHP_INT_MAX],
    ]]);
}, MongoDB\Driver\Exception\InvalidArgumentException::class), "\n";

// suffix.strMaxQueryLength out of range
echo throws(function() use ($clientEncryption, $baseOpts) {
    $clientEncryption->encrypt('test', $baseOpts + ['stringOpts' => [
        'suffix' => ['strMaxQueryLength' => -1],
    ]]);
}, MongoDB\Driver\Exception\InvalidArgumentException::class), "\n";

// suffix.strMinQueryLength out of range
echo throws(function() use ($clientEncryption, $baseOpts) {
    $clientEncryption->encrypt('test', $baseOpts + ['stringOpts' => [
        'suffix' => ['strMinQueryLength' => PHP_INT_MAX],
    ]]);
}, MongoDB\Driver\Exception\InvalidArgumentException::class), "\n";

// substring.strMaxLength out of range
echo throws(function() use ($clientEncryption, $baseOpts) {
    $clientEncryption->encrypt('test', $baseOpts + ['stringOpts' => [
        'substring' => ['strMaxLength' => -1],
    ]]);
}, MongoDB\Driver\Exception\InvalidArgumentException::class), "\n";

// substring.strMaxQueryLength out of range
echo throws(function() use ($clientEncryption, $baseOpts) {
    $clientEncryption->encrypt('test', $baseOpts + ['stringOpts' => [
        'substring' => ['strMaxQueryLength' => -1],
    ]]);
}, MongoDB\Driver\Exception\InvalidArgumentException::class), "\n";

// substring.strMinQueryLength out of range
echo throws(function() use ($clientEncryption, $baseOpts) {
    $clientEncryption->encrypt('test', $baseOpts + ['stringOpts' => [
        'substring' => ['strMinQueryLength' => PHP_INT_MAX],
    ]]);
}, MongoDB\Driver\Exception\InvalidArgumentException::class), "\n";

?>
===DONE===
<?php exit(0); ?>
--EXPECT--
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Expected "strMaxQueryLength" to be a positive 32-bit integer, -1 given
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Expected "strMinQueryLength" to be a positive 32-bit integer, 9223372036854775807 given
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Expected "strMaxQueryLength" to be a positive 32-bit integer, -1 given
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Expected "strMinQueryLength" to be a positive 32-bit integer, 9223372036854775807 given
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Expected "strMaxLength" to be a positive 32-bit integer, -1 given
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Expected "strMaxQueryLength" to be a positive 32-bit integer, -1 given
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Expected "strMinQueryLength" to be a positive 32-bit integer, 9223372036854775807 given
===DONE===