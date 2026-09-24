--TEST--
MongoDB\Driver\WriteConcern debug output
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
    // 64-bit wtimeout may be reported as integer or string
    MongoDB\Driver\WriteConcern::__set_state(['w' => 2, 'wtimeout' => '2147483648']),
];

foreach ($tests as $test) {
    var_dump($test);
}

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
  NULL
  ["j"]=>
  NULL
  ["wtimeout"]=>
  int(0)
}
object(MongoDB\Driver\WriteConcern)#%d (3) {
  ["w"]=>
  int(-1)
  ["j"]=>
  NULL
  ["wtimeout"]=>
  int(0)
}
object(MongoDB\Driver\WriteConcern)#%d (3) {
  ["w"]=>
  int(0)
  ["j"]=>
  NULL
  ["wtimeout"]=>
  int(0)
}
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
  string(8) "majority"
  ["j"]=>
  NULL
  ["wtimeout"]=>
  int(0)
}
object(MongoDB\Driver\WriteConcern)#%d (3) {
  ["w"]=>
  string(3) "tag"
  ["j"]=>
  NULL
  ["wtimeout"]=>
  int(0)
}
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
  int(1)
  ["j"]=>
  bool(false)
  ["wtimeout"]=>
  int(0)
}
object(MongoDB\Driver\WriteConcern)#%d (3) {
  ["w"]=>
  int(1)
  ["j"]=>
  NULL
  ["wtimeout"]=>
  int(1000)
}
object(MongoDB\Driver\WriteConcern)#%d (3) {
  ["w"]=>
  int(1)
  ["j"]=>
  bool(true)
  ["wtimeout"]=>
  int(1000)
}
object(MongoDB\Driver\WriteConcern)#%d (3) {
  ["w"]=>
  NULL
  ["j"]=>
  bool(true)
  ["wtimeout"]=>
  int(0)
}
object(MongoDB\Driver\WriteConcern)#%d (3) {
  ["w"]=>
  NULL
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
  %rint\(2147483648\)|int\(2147483647\)%r
}
===DONE===
