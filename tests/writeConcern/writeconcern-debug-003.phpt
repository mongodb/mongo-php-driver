--TEST--
MongoDB\Driver\WriteConcern debug output
--FILE--
<?php

require_once __DIR__ . '/../utils/tools.php';

$tests = [
    new MongoDB\Driver\WriteConcern(-3), // MONGOC_WRITE_CONCERN_W_MAJORITY
    new MongoDB\Driver\WriteConcern(-2), // MONGOC_WRITE_CONCERN_W_DEFAULT
    new MongoDB\Driver\WriteConcern(-1),
    new MongoDB\Driver\WriteConcern(0),
    new MongoDB\Driver\WriteConcern(1),
    new MongoDB\Driver\WriteConcern(MongoDB\Driver\WriteConcern::MAJORITY),
    new MongoDB\Driver\WriteConcern('tag'),
    new MongoDB\Driver\WriteConcern(1, 0),
    new MongoDB\Driver\WriteConcern(1, 0, false),
    new MongoDB\Driver\WriteConcern(1, 1000),
    new MongoDB\Driver\WriteConcern(1, 1000, true),
    new MongoDB\Driver\WriteConcern(-2, 0, true),
    // Note: wtimeout is only applicable applies for w > 1
    new MongoDB\Driver\WriteConcern(-2, 1000),
];

foreach ($tests as $test) {
    var_dump($test);
}

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
object(MongoDB\Driver\WriteConcern)#%d (%d) {
  ["w"]=>
  string(8) "majority"
}
object(MongoDB\Driver\WriteConcern)#%d (%d) {
}
object(MongoDB\Driver\WriteConcern)#%d (%d) {
  ["w"]=>
  int(-1)
}
object(MongoDB\Driver\WriteConcern)#%d (%d) {
  ["w"]=>
  int(0)
}
object(MongoDB\Driver\WriteConcern)#%d (%d) {
  ["w"]=>
  int(1)
}
object(MongoDB\Driver\WriteConcern)#%d (%d) {
  ["w"]=>
  string(8) "majority"
}
object(MongoDB\Driver\WriteConcern)#%d (%d) {
  ["w"]=>
  string(3) "tag"
}
object(MongoDB\Driver\WriteConcern)#%d (%d) {
  ["w"]=>
  int(1)
}
object(MongoDB\Driver\WriteConcern)#%d (%d) {
  ["w"]=>
  int(1)
  ["j"]=>
  bool(false)
}
object(MongoDB\Driver\WriteConcern)#%d (%d) {
  ["w"]=>
  int(1)
  ["wtimeout"]=>
  int(1000)
}
object(MongoDB\Driver\WriteConcern)#%d (%d) {
  ["w"]=>
  int(1)
  ["j"]=>
  bool(true)
  ["wtimeout"]=>
  int(1000)
}
object(MongoDB\Driver\WriteConcern)#%d (%d) {
  ["j"]=>
  bool(true)
}
object(MongoDB\Driver\WriteConcern)#%d (%d) {
  ["wtimeout"]=>
  int(1000)
}
===DONE===
