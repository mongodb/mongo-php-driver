--TEST--
MongoDB\Driver\WriteConcern construction
--FILE--
<?php

var_dump(new MongoDB\Driver\WriteConcern(MongoDB\Driver\WriteConcern::MAJORITY));
var_dump(new MongoDB\Driver\WriteConcern(MongoDB\Driver\WriteConcern::MAJORITY, 1000));

var_dump(new MongoDB\Driver\WriteConcern(2));
var_dump(new MongoDB\Driver\WriteConcern(2, 2000));

var_dump(new MongoDB\Driver\WriteConcern("tagname"));
var_dump(new MongoDB\Driver\WriteConcern("string", 3000));

var_dump(new MongoDB\Driver\WriteConcern("string", 4000, true));
var_dump(new MongoDB\Driver\WriteConcern("string", 5000, false));
var_dump(new MongoDB\Driver\WriteConcern("string", 6000, null));

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
object(MongoDB\Driver\WriteConcern)#%d (3) {
  ["w"]=>
  string(8) "majority"
  ["j"]=>
  NULL
  ["wtimeout"]=>
  int(0)
}
object(MongoDB\Driver\WriteConcern)#%d (3) {
  ["w"]=>
  string(8) "majority"
  ["j"]=>
  NULL
  ["wtimeout"]=>
  int(1000)
}
object(MongoDB\Driver\WriteConcern)#%d (3) {
  ["w"]=>
  int(2)
  ["j"]=>
  NULL
  ["wtimeout"]=>
  int(0)
}
object(MongoDB\Driver\WriteConcern)#%d (3) {
  ["w"]=>
  int(2)
  ["j"]=>
  NULL
  ["wtimeout"]=>
  int(2000)
}
object(MongoDB\Driver\WriteConcern)#%d (3) {
  ["w"]=>
  string(7) "tagname"
  ["j"]=>
  NULL
  ["wtimeout"]=>
  int(0)
}
object(MongoDB\Driver\WriteConcern)#%d (3) {
  ["w"]=>
  string(6) "string"
  ["j"]=>
  NULL
  ["wtimeout"]=>
  int(3000)
}
object(MongoDB\Driver\WriteConcern)#%d (3) {
  ["w"]=>
  string(6) "string"
  ["j"]=>
  bool(true)
  ["wtimeout"]=>
  int(4000)
}
object(MongoDB\Driver\WriteConcern)#%d (3) {
  ["w"]=>
  string(6) "string"
  ["j"]=>
  bool(false)
  ["wtimeout"]=>
  int(5000)
}
object(MongoDB\Driver\WriteConcern)#%d (3) {
  ["w"]=>
  string(6) "string"
  ["j"]=>
  NULL
  ["wtimeout"]=>
  int(6000)
}
===DONE===
