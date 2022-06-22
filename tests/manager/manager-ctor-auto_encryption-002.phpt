--TEST--
MongoDB\Driver\Manager::__construct(): crypt_shared is required
--SKIPIF--
<?php require __DIR__ . "/../utils/basic-skipif.inc"; ?>
<?php skip_if_not_libmongocrypt(); ?>
<?php skip_if_no_crypt_shared(); ?>
--FILE--
<?php

require_once __DIR__ . '/../utils/basic.inc';

$autoEncryptionOptions = [
    'keyVaultNamespace' => CSFLE_KEY_VAULT_NS,
    'kmsProviders' => ['local' => ['key' => new MongoDB\BSON\Binary(CSFLE_LOCAL_KEY, 0)]],
    'extraOptions' => ['cryptSharedLibRequired' => true],
];

create_test_manager(null, [], ['autoEncryption' => $autoEncryptionOptions]);

?>
===DONE===
<?php exit(0); ?>
--EXPECT--
===DONE===
