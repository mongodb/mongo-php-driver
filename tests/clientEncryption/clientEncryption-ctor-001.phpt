--TEST--
MongoDB\Driver\ClientEncryption::__construct()
--SKIPIF--
<?php require __DIR__ . "/../utils/basic-skipif.inc"; ?>
<?php skip_if_not_libmongocrypt(); ?>
--FILE--
<?php

require_once __DIR__ . "/../utils/basic.inc";

$clientEncryption = new MongoDB\Driver\ClientEncryption([
    'keyVaultClient' => create_test_manager(),
    'keyVaultNamespace' => CSFLE_KEY_VAULT_NS,
    'kmsProviders' => ['local' => ['key' => new MongoDB\BSON\Binary(CSFLE_LOCAL_KEY, 0)]],
]);

var_dump($clientEncryption);

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
object(MongoDB\Driver\ClientEncryption)#%d (%d) {
}
===DONE===
