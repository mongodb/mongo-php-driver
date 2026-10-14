--TEST--
MongoDB\Driver\WriteConcern debug output
--FILE--
<?php

var_dump(new MongoDB\Driver\WriteConcern(1));
var_dump(new MongoDB\Driver\WriteConcern("tag", 1000, false));
var_dump(new MongoDB\Driver\WriteConcern(MongoDB\Driver\WriteConcern::MAJORITY, 500, true));

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
object(MongoDB\Driver\WriteConcern)#%d (3) {
  ["w"]=>
  int(1)
  ["j"]=>
  NULL
  ["wtimeout"]=>
  int(0)
}
object(MongoDB\Driver\WriteConcern)#%d (3) {
  ["w"]=>
  string(3) "tag"
  ["j"]=>
  bool(false)
  ["wtimeout"]=>
  int(1000)
}
object(MongoDB\Driver\WriteConcern)#%d (3) {
  ["w"]=>
  string(8) "majority"
  ["j"]=>
  bool(true)
  ["wtimeout"]=>
  int(500)
}
===DONE===
