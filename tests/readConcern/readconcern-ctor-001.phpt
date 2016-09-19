--TEST--
MongoDB\Driver\ReadConcern construction
--SKIPIF--
<?php require __DIR__ . "/../utils/basic-skipif.inc"?>
--FILE--
<?php
require_once __DIR__ . "/../utils/basic.inc";

var_dump(new MongoDB\Driver\ReadConcern());
var_dump(new MongoDB\Driver\ReadConcern(null));
var_dump(new MongoDB\Driver\ReadConcern(MongoDB\Driver\ReadConcern::LOCAL));
var_dump(new MongoDB\Driver\ReadConcern(MongoDB\Driver\ReadConcern::MAJORITY));
var_dump(new MongoDB\Driver\ReadConcern('not-yet-supported'));

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
object(MongoDB\Driver\ReadConcern)#%d (%d) {
}
object(MongoDB\Driver\ReadConcern)#%d (%d) {
}
object(MongoDB\Driver\ReadConcern)#%d (%d) {
  ["level"]=>
  string(5) "local"
}
object(MongoDB\Driver\ReadConcern)#%d (%d) {
  ["level"]=>
  string(8) "majority"
}
object(MongoDB\Driver\ReadConcern)#%d (%d) {
  ["level"]=>
  string(17) "not-yet-supported"
}
===DONE===
