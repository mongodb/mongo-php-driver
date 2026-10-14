--TEST--
MongoDB\Driver\ClientEncryption::encrypt() with stringOpts (substringPreview)
--SKIPIF--
<?php require __DIR__ . "/../utils/basic-skipif.inc"; ?>
<?php skip_if_not_libmongocrypt(); ?>
<?php skip_if_not_live(); ?>
<?php skip_if_standalone(); ?>
<?php skip_if_server_version('<', '8.2'); ?>
<?php skip_if_not_clean(CSFLE_KEY_VAULT_DATABASE_NAME, CSFLE_KEY_VAULT_COLLECTION_NAME); ?>
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
    'algorithm' => 'String',
    'queryType' => 'substringPreview',
    'contentionFactor' => 0,
    'stringOpts' => [
        'caseSensitive' => true,
        'diacriticSensitive' => true,
        'substring' => ['strMaxLength' => 10, 'strMaxQueryLength' => 10, 'strMinQueryLength' => 2],
    ],
];

$encrypted = $clientEncryption->encrypt('foobar', $encryptOpts);

var_dump($encrypted instanceof MongoDB\BSON\Binary);
var_dump($encrypted->getType() === MongoDB\BSON\Binary::TYPE_ENCRYPTED);

?>
===DONE===
<?php exit(0); ?>
--EXPECT--
bool(true)
bool(true)
===DONE===