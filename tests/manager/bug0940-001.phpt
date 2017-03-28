--TEST--
PHPC-940: php_phongo_free_ssl_opt() attempts to free interned strings
--SKIPIF--
<?php require __DIR__ . "/../utils/basic-skipif.inc"; ?>
--FILE--
<?php

var_dump(new MongoDB\Driver\Manager(null, [], ['ca_file' => false]));

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
object(MongoDB\Driver\Manager)#%d (%d) {
  ["uri"]=>
  string(20) "mongodb://127.0.0.1/"
  ["cluster"]=>
  array(0) {
  }
}
===DONE===
