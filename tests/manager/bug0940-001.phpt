--TEST--
PHPC-940: php_phongo_free_ssl_opt() attempts to free interned strings
--SKIPIF--
<?php require __DIR__ . "/../utils/basic-skipif.inc"; ?>
<?php skip_if_not_libmongoc_ssl(); /* SSL opts are ignored without MONGOC_ENABLE_SSL */ ?>
--FILE--
<?php

var_dump(new MongoDB\Driver\Manager(null, [], ['ca_file' => false]));

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
Deprecated: MongoDB\Driver\Manager::__construct(): The "ca_file" driver option is deprecated. Please use the "tlsCAFile" URI option instead.%s
object(MongoDB\Driver\Manager)#%d (%d) {
  ["uri"]=>
  string(20) "mongodb://127.0.0.1/"
  ["cluster"]=>
  array(0) {
  }
}
===DONE===
