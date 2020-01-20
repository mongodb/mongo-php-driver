--TEST--
MongoDB\Driver\Manager::__construct(): auto encryption options
--SKIPIF--
<?php require __DIR__ . "/../utils/basic-skipif.inc"; ?>
<?php skip_if_not_libmongocrypt(); ?>
--FILE--
<?php

$baseOptions = [
    'keyVaultNamespace' => 'admin.dataKeys',
    'kmsProviders' => ['aws' => (object) ['accessKeyId' => 'abc', 'secretAccessKey' => 'def']]
];

$tests = [
    [],
    ['keyVaultClient' => new MongoDB\Driver\Manager()],
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
    ['bypassAutoEncryption' => true],
    ['extraOptions' => ['mongocryptdBypassSpawn' => true]],
];

foreach ($tests as $autoEncryptionOptions) {
    $manager = new MongoDB\Driver\Manager(null, [], ['autoEncryption' => $autoEncryptionOptions + $baseOptions]);
}

?>
===DONE===
<?php exit(0); ?>
--EXPECT--
===DONE===
