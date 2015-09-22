--TEST--
MongoDB\Driver\WriteConcern debug output should include all fields for w default
--SKIPIF--
<?php require __DIR__ . "/../utils/basic-skipif.inc"?>
--FILE--
<?php
require_once __DIR__ . "/../utils/basic.inc";

/* -2 is libmongoc's constant for relying on the server's default value for "w".
 * Although "w" will be omitted from the write concern sent to the server, we
 * should still yield other fields in the debug output, which may be sent.
 */
var_dump(new MongoDB\Driver\WriteConcern(-2, 1000, true));

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
object(MongoDB\Driver\WriteConcern)#%d (%d) {
  ["w"]=>
  NULL
  ["wmajority"]=>
  bool(false)
  ["wtimeout"]=>
  int(1000)
  ["journal"]=>
  bool(true)
}
===DONE===
