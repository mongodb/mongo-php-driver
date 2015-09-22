--TEST--
MongoDB\Driver\WriteConcern debug output
--SKIPIF--
<?php require __DIR__ . "/../utils/basic-skipif.inc"?>
--FILE--
<?php
require_once __DIR__ . "/../utils/basic.inc";

var_dump(new MongoDB\Driver\WriteConcern(1));
var_dump(new MongoDB\Driver\WriteConcern("tag", 1000, false));
var_dump(new MongoDB\Driver\WriteConcern(MongoDB\Driver\WriteConcern::MAJORITY, 500, true));

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
object(MongoDB\Driver\WriteConcern)#%d (%d) {
  ["w"]=>
  int(1)
  ["wmajority"]=>
  bool(false)
  ["wtimeout"]=>
  int(0)
  ["journal"]=>
  NULL
}
object(MongoDB\Driver\WriteConcern)#%d (%d) {
  ["w"]=>
  string(3) "tag"
  ["wmajority"]=>
  bool(false)
  ["wtimeout"]=>
  int(1000)
  ["journal"]=>
  bool(false)
}
object(MongoDB\Driver\WriteConcern)#%d (%d) {
  ["w"]=>
  string(8) "majority"
  ["wmajority"]=>
  bool(true)
  ["wtimeout"]=>
  int(500)
  ["journal"]=>
  bool(true)
}
===DONE===
