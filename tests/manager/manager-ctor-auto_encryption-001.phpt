--TEST--
MongoDB\Driver\Manager::__construct(): autoEncryption options
--SKIPIF--
<?php require __DIR__ . "/../utils/basic-skipif.inc"; ?>
<?php skip_if_not_libmongocrypt(); ?>
<?php skip_if_crypt_shared(); // Build fails due to SERVER-71049 ?>
--FILE--
<?php

require_once __DIR__ . '/../utils/basic.inc';

$baseOptions = [
    'keyVaultNamespace' => CSFLE_KEY_VAULT_NS,
    'kmsProviders' => ['local' => ['key' => new MongoDB\BSON\Binary(CSFLE_LOCAL_KEY, 0)]],
    /* Testing the schemaMap option requires either crypt_shared or mongocryptd
     * for automatic encryption. Disable mongocryptd spawning to avoid a test
     * failure if crypt_shared and mongocryptd are both unavailable. */
    'extraOptions' => ['mongocryptdBypassSpawn' => true],
];

$tests = [
    [],
    ['bypassAutoEncryption' => true],
    ['bypassQueryAnalysis' => true],
    ['keyVaultClient' => create_test_manager()],
    ['schemaMap' => [
        'default.default' => [
            'properties' => [
                'encrypted_objectId' => [
                    'encrypt' => [
                        'keyId' => [
                            [
                                '$binary' => [
                                    'base64' => 'AAAAAAAAAAAAAAAAAAAAAA==',
                                    'subType' => '04',
                                ],
                            ],
                        ],
                        'bsonType' => 'objectId',
                        'algorithm' => MongoDB\Driver\ClientEncryption::AEAD_AES_256_CBC_HMAC_SHA_512_DETERMINISTIC,
                    ],
                ],
            ],
            'bsonType' => 'object',
        ],
    ]],
];

foreach ($tests as $autoEncryptionOptions) {
    create_test_manager(null, [], ['autoEncryption' => $autoEncryptionOptions + $baseOptions]);
}

?>
===DONE===
<?php exit(0); ?>
--EXPECT--
===DONE===
