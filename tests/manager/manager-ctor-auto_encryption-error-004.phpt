--TEST--
MongoDB\Driver\Manager::__construct(): crypt_shared is required and unavailable
--SKIPIF--
<?php require __DIR__ . "/../utils/basic-skipif.inc"; ?>
<?php skip_if_not_libmongocrypt(); ?>
<?php skip_if_crypt_shared(); ?>
--FILE--
<?php

require_once __DIR__ . '/../utils/basic.inc';

$baseOptions = [
    'keyVaultNamespace' => CSFLE_KEY_VAULT_NS,
    'kmsProviders' => ['local' => ['key' => new MongoDB\BSON\Binary(CSFLE_LOCAL_KEY, 0)]],
];

$tests = [
    ['extraOptions' => ['cryptSharedLibPath' => '/not/found']],
    ['extraOptions' => ['cryptSharedLibRequired' => true]],
];

foreach ($tests as $autoEncryptionOptions) {
    echo throws(function() use ($autoEncryptionOptions, $baseOptions) {
        create_test_manager(null, [], ['autoEncryption' => $autoEncryptionOptions + $baseOptions]);
    }, MongoDB\Driver\Exception\EncryptionException::class), "\n\n";
}

var_dump(get_module_info('crypt_shared library version'));

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
OK: Got MongoDB\Driver\Exception\EncryptionException
A crypt_shared override path was specified [/not/found], but we failed to open a dynamic library at that location%A

OK: Got MongoDB\Driver\Exception\EncryptionException
Option 'cryptSharedLibRequired' is 'true', but failed to load the crypt_shared runtime library

string(7) "unknown"
===DONE===
