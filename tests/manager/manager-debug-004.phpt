--TEST--
MongoDB\Driver\Manager debug output with crypt_shared
--SKIPIF--
<?php require __DIR__ . "/../utils/basic-skipif.inc"; ?>
<?php skip_if_not_libmongocrypt(); ?>
<?php skip_if_no_crypt_shared(); ?>
--FILE--
<?php

require_once __DIR__ . '/../utils/basic.inc';

// First, create a test manager without auto-encryption
$manager = create_test_manager();

var_dump($manager);
var_dump(get_module_info('crypt_shared library version'));

$autoEncryptionOptions = [
    'keyVaultNamespace' => CSFLE_KEY_VAULT_NS,
    'kmsProviders' => ['local' => ['key' => new MongoDB\BSON\Binary(CSFLE_LOCAL_KEY, 0)]],
    'extraOptions' => ['cryptSharedLibRequired' => true],
];

$manager = create_test_manager(null, [], ['autoEncryption' => $autoEncryptionOptions]);

var_dump($manager);
var_dump(get_module_info('crypt_shared library version'));

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
object(MongoDB\Driver\Manager)#%d (%d) {
  ["uri"]=>
  string(%d) "%s"
  ["cluster"]=>
  array(0) {
  }
  ["cryptSharedVersion"]=>
  NULL
}
string(7) "unknown"
object(MongoDB\Driver\Manager)#%d (%d) {
  ["uri"]=>
  string(%d) "%s"
  ["cluster"]=>
  array(0) {
  }
  ["cryptSharedVersion"]=>
  string(%d) "mongo_crypt%a"
}
string(7) "unknown"
===DONE===
