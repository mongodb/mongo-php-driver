--TEST--
MongoDB\Driver\WriteConcern serialization (__serialize and __unserialize)
--SKIPIF--
<?php require __DIR__ . "/../utils/basic-skipif.inc"; ?>
<?php skip_if_php_version('<', '7.4.0'); ?>
--FILE--
<?php

require_once __DIR__ . '/../utils/basic.inc';

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
    // 64-bit wtimeout is always encoded as as string
    MongoDB\Driver\WriteConcern::__set_state(['w' => 2, 'wtimeout' => '2147483648']),
];

foreach ($tests as $test) {
    var_dump($test);
    echo $s = serialize($test), "\n";
    var_dump(unserialize($s));
    echo "\n";
}

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
object(MongoDB\Driver\WriteConcern)#%d (%d) {
  ["w"]=>
  string(8) "majority"
}
O:27:"MongoDB\Driver\WriteConcern":1:{s:1:"w";s:8:"majority";}
object(MongoDB\Driver\WriteConcern)#%d (%d) {
  ["w"]=>
  string(8) "majority"
}

object(MongoDB\Driver\WriteConcern)#%d (%d) {
}
O:27:"MongoDB\Driver\WriteConcern":0:{}
object(MongoDB\Driver\WriteConcern)#%d (%d) {
}

object(MongoDB\Driver\WriteConcern)#%d (%d) {
  ["w"]=>
  int(-1)
}
O:27:"MongoDB\Driver\WriteConcern":1:{s:1:"w";i:-1;}
object(MongoDB\Driver\WriteConcern)#%d (%d) {
  ["w"]=>
  int(-1)
}

object(MongoDB\Driver\WriteConcern)#%d (%d) {
  ["w"]=>
  int(0)
}
O:27:"MongoDB\Driver\WriteConcern":1:{s:1:"w";i:0;}
object(MongoDB\Driver\WriteConcern)#%d (%d) {
  ["w"]=>
  int(0)
}

object(MongoDB\Driver\WriteConcern)#%d (%d) {
  ["w"]=>
  int(1)
}
O:27:"MongoDB\Driver\WriteConcern":1:{s:1:"w";i:1;}
object(MongoDB\Driver\WriteConcern)#%d (%d) {
  ["w"]=>
  int(1)
}

object(MongoDB\Driver\WriteConcern)#%d (%d) {
  ["w"]=>
  string(8) "majority"
}
O:27:"MongoDB\Driver\WriteConcern":1:{s:1:"w";s:8:"majority";}
object(MongoDB\Driver\WriteConcern)#%d (%d) {
  ["w"]=>
  string(8) "majority"
}

object(MongoDB\Driver\WriteConcern)#%d (%d) {
  ["w"]=>
  string(3) "tag"
}
O:27:"MongoDB\Driver\WriteConcern":1:{s:1:"w";s:3:"tag";}
object(MongoDB\Driver\WriteConcern)#%d (%d) {
  ["w"]=>
  string(3) "tag"
}

object(MongoDB\Driver\WriteConcern)#%d (%d) {
  ["w"]=>
  int(1)
}
O:27:"MongoDB\Driver\WriteConcern":1:{s:1:"w";i:1;}
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
O:27:"MongoDB\Driver\WriteConcern":2:{s:1:"w";i:1;s:1:"j";b:0;}
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
O:27:"MongoDB\Driver\WriteConcern":2:{s:1:"w";i:1;s:8:"wtimeout";i:1000;}
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
O:27:"MongoDB\Driver\WriteConcern":3:{s:1:"w";i:1;s:1:"j";b:1;s:8:"wtimeout";i:1000;}
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
O:27:"MongoDB\Driver\WriteConcern":1:{s:1:"j";b:1;}
object(MongoDB\Driver\WriteConcern)#%d (%d) {
  ["j"]=>
  bool(true)
}

object(MongoDB\Driver\WriteConcern)#%d (%d) {
  ["wtimeout"]=>
  int(1000)
}
O:27:"MongoDB\Driver\WriteConcern":1:{s:8:"wtimeout";i:1000;}
object(MongoDB\Driver\WriteConcern)#%d (%d) {
  ["wtimeout"]=>
  int(1000)
}

object(MongoDB\Driver\WriteConcern)#%d (%d) {
  ["w"]=>
  int(2)
  ["wtimeout"]=>
  %rint\(2147483648\)|string\(10\) "2147483648"%r
}
O:27:"MongoDB\Driver\WriteConcern":2:{s:1:"w";i:2;s:8:"wtimeout";s:10:"2147483648";}
object(MongoDB\Driver\WriteConcern)#%d (%d) {
  ["w"]=>
  int(2)
  ["wtimeout"]=>
  %rint\(2147483648\)|string\(10\) "2147483648"%r
}

===DONE===
