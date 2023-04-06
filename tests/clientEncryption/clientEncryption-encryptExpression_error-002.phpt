--TEST--
MongoDB\Driver\ClientEncryption::encryptExpression() requires $and operator
--SKIPIF--
<?php require __DIR__ . "/../utils/basic-skipif.inc"; ?>
<?php skip_if_not_libmongocrypt(); ?>
<?php skip_if_not_live(); ?>
<?php skip_if_server_version('<', '7.0'); ?>
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

$encryptOpts = [
    'keyId' => $keyId,
    'algorithm' => MongoDB\Driver\ClientEncryption::ALGORITHM_RANGE_PREVIEW,
    'queryType' => MongoDB\Driver\ClientEncryption::QUERY_TYPE_RANGE_PREVIEW,
    'contentionFactor' => 0,
    'rangeOpts' => ['min' => 0, 'max' => 200, 'sparsity' => 1],
];

echo throws(function() use ($clientEncryption, $encryptOpts) {
    $clientEncryption->encryptExpression([], $encryptOpts);
}, MongoDB\Driver\Exception\EncryptionException::class), "\n";

?>
===DONE===
<?php exit(0); ?>
--EXPECT--
OK: Got MongoDB\Driver\Exception\EncryptionException
error unable to find '$and': { }
===DONE===
