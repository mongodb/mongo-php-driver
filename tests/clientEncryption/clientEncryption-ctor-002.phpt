--TEST--
MongoDB\Driver\ClientEncryption::__construct(): Drivers MUST NOT error if tlsDisableOCSPEndpointCheck is set
--SKIPIF--
<?php require __DIR__ . "/../utils/basic-skipif.inc"; ?>
<?php skip_if_not_libmongocrypt(); ?>
--FILE--
<?php

require_once __DIR__ . "/../utils/basic.inc";

$clientEncryption = new MongoDB\Driver\ClientEncryption([
    'keyVaultClient' => create_test_manager(),
    'keyVaultNamespace' => CSFLE_KEY_VAULT_NS,
    'kmsProviders' => ['aws' => ['accessKeyId' => 'foo', 'secretAccessKey' => 'bar']],
    'tlsOptions' => ['aws' => ['tlsDisableOCSPEndpointCheck' => true]],
]);

var_dump($clientEncryption);

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
object(MongoDB\Driver\ClientEncryption)#%d (%d) {
}
===DONE===
