--TEST--
MongoDB\Driver\Manager with disableClientPersistence=true referenced by ClientEncryption (implicit keyVaultClient)
--SKIPIF--
<?php require __DIR__ . "/../utils/basic-skipif.inc"; ?>
<?php skip_if_not_libmongocrypt(); ?>
--FILE--
<?php
require_once __DIR__ . "/../utils/basic.inc";

ini_set('mongodb.debug', 'stderr');
$manager = create_test_manager(null, [], ['disableClientPersistence' => true]);
ini_set('mongodb.debug', '');

echo "Creating clientEncryption\n";
$clientEncryption = $manager->createClientEncryption([
    'keyVaultNamespace' => CSFLE_KEY_VAULT_NS,
    'kmsProviders' => ['local' => ['key' => new MongoDB\BSON\Binary(CSFLE_LOCAL_KEY, 0)]],
]);

echo "Unsetting manager\n";
ini_set('mongodb.debug', 'stderr');
unset($manager);
ini_set('mongodb.debug', '');

echo "Unsetting clientEncryption\n";
ini_set('mongodb.debug', 'stderr');
unset($clientEncryption);
ini_set('mongodb.debug', '');

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
%A
[%s]     PHONGO: DEBUG   > Created client with hash: %s
[%s]     PHONGO: DEBUG   > Stored non-persistent client
Creating clientEncryption
Unsetting manager
Unsetting clientEncryption%A
[%s]     PHONGO: DEBUG   > Destroying non-persistent client for Manager%A
===DONE===
