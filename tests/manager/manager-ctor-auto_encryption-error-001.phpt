--TEST--
MongoDB\Driver\Manager::__construct(): incomplete autoEncryption options
--SKIPIF--
<?php require __DIR__ . "/../utils/basic-skipif.inc"; ?>
<?php skip_if_not_libmongocrypt(); ?>
--FILE--
<?php
require_once __DIR__ . '/../utils/basic.inc';

$tests = [
    [],
    ['keyVaultNamespace' => CSFLE_KEY_VAULT_NS],
];

foreach ($tests as $autoEncryptionOptions) {
    echo throws(function() use ($autoEncryptionOptions) {
        create_test_manager(null, [], ['autoEncryption' => $autoEncryptionOptions]);
    }, MongoDB\Driver\Exception\InvalidArgumentException::class), "\n\n";
}

?>
===DONE===
<?php exit(0); ?>
--EXPECT--
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Key vault namespace option required

OK: Got MongoDB\Driver\Exception\InvalidArgumentException
KMS providers option required

===DONE===
