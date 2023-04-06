--TEST--
MongoDB\Driver\ClientEncryption::encryptExpression() encrypts $and operator
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

$expr = [
    '$and' => [
        ['encryptedInt' => ['$gte' => 6]],
        ['encryptedInt' => ['$lte' => 200]],
    ],
];

$encryptedExpr = $clientEncryption->encryptExpression($expr, $encryptOpts);

var_dump($encryptedExpr->{'$and'}[0]->encryptedInt->{'$gte'} instanceof MongoDB\BSON\Binary);
var_dump($encryptedExpr->{'$and'}[0]->encryptedInt->{'$gte'}->getType() === MongoDB\BSON\Binary::TYPE_ENCRYPTED);

var_dump($encryptedExpr->{'$and'}[1]->encryptedInt->{'$lte'} instanceof MongoDB\BSON\Binary);
var_dump($encryptedExpr->{'$and'}[1]->encryptedInt->{'$lte'}->getType() === MongoDB\BSON\Binary::TYPE_ENCRYPTED);

?>
===DONE===
<?php exit(0); ?>
--EXPECT--
bool(true)
bool(true)
bool(true)
bool(true)
===DONE===
