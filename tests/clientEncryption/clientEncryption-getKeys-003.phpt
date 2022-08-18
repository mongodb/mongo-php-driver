--TEST--
MongoDB\Driver\ClientEncryption::getKeys() cursor debug info
--SKIPIF--
<?php require __DIR__ . "/../utils/basic-skipif.inc"; ?>
<?php skip_if_not_libmongocrypt(); ?>
<?php skip_if_not_live(); ?>
<?php skip_if_server_version('<', '4.2'); ?>
<?php skip_if_not_clean(CSFLE_KEY_VAULT_DATABASE_NAME, CSFLE_KEY_VAULT_COLLECTION_NAME);
--FILE--
<?php

require_once __DIR__ . "/../utils/basic.inc";

$manager = create_test_manager();

$clientEncryption = $manager->createClientEncryption([
  'keyVaultNamespace' => CSFLE_KEY_VAULT_NS,
  'kmsProviders' => ['local' => ['key' => new MongoDB\BSON\Binary(CSFLE_LOCAL_KEY, 0)]],
]);

/* Note: the query reports a null readConcern property despite libmongoc using
 * a majority read concern internally (as it does for all key vault reads). */
var_dump($clientEncryption->getKeys());

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
object(MongoDB\Driver\Cursor)#%d (%d) {
  ["database"]=>
  string(%d) "%s"
  ["collection"]=>
  string(%d) "%s"
  ["query"]=>
  object(MongoDB\Driver\Query)#%d (%d) {
    ["filter"]=>
    object(stdClass)#%d (%d) {
    }
    ["options"]=>
    object(stdClass)#%d (%d) {
    }
    ["readConcern"]=>
    NULL
  }
  ["command"]=>
  NULL
  ["readPreference"]=>
  NULL
  ["session"]=>
  NULL
  ["isDead"]=>
  bool(true)
  ["currentIndex"]=>
  int(0)
  ["currentDocument"]=>
  NULL
  ["server"]=>
  object(MongoDB\Driver\Server)#%d (%d) {
    %a
  }
}
===DONE===
