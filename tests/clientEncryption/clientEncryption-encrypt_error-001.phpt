--TEST--
MongoDB\Driver\ClientEncryption::encrypt() BSON encoding errors
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

class SerializableError implements MongoDB\BSON\Serializable {
    #[\ReturnTypeWillChange]
    public function bsonSerialize()
    {
        throw new RuntimeException('bsonSerialize() error');
    }
}

echo throws(function() use ($clientEncryption) {
    $clientEncryption->encrypt(new SerializableError());
}, RuntimeException::class), "\n";

echo throws(function() use ($clientEncryption) {
    $clientEncryption->encrypt('top-secret', ['keyId' => new SerializableError()]);
}, RuntimeException::class), "\n";

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
OK: Got RuntimeException
bsonSerialize() error
OK: Got RuntimeException
bsonSerialize() error
===DONE===
