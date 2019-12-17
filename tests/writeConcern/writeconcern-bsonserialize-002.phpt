--TEST--
MongoDB\Driver\WriteConcern::bsonSerialize() returns an object
--FILE--
<?php

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
    // Note: wtimeout is only applicable for w > 1
    new MongoDB\Driver\WriteConcern(-2, 1000),
];

foreach ($tests as $test) {
    var_dump($test->bsonSerialize());
}

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
object(stdClass)#%d (%d) {
  ["w"]=>
  string(8) "majority"
}
object(stdClass)#%d (%d) {
}
object(stdClass)#%d (%d) {
  ["w"]=>
  int(-1)
}
object(stdClass)#%d (%d) {
  ["w"]=>
  int(0)
}
object(stdClass)#%d (%d) {
  ["w"]=>
  int(1)
}
object(stdClass)#%d (%d) {
  ["w"]=>
  string(8) "majority"
}
object(stdClass)#%d (%d) {
  ["w"]=>
  string(3) "tag"
}
object(stdClass)#%d (%d) {
  ["w"]=>
  int(1)
}
object(stdClass)#%d (%d) {
  ["w"]=>
  int(1)
  ["j"]=>
  bool(false)
}
object(stdClass)#%d (%d) {
  ["w"]=>
  int(1)
  ["wtimeout"]=>
  int(1000)
}
object(stdClass)#%d (%d) {
  ["w"]=>
  int(1)
  ["j"]=>
  bool(true)
  ["wtimeout"]=>
  int(1000)
}
object(stdClass)#%d (%d) {
  ["j"]=>
  bool(true)
}
object(stdClass)#%d (%d) {
  ["wtimeout"]=>
  int(1000)
}
===DONE===
