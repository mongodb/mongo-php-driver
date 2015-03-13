--TEST--
MongoDB\Driver\WriteConcern construction
--SKIPIF--
<?php require "tests/utils/basic-skipif.inc"?>
--FILE--
<?php
require_once "tests/utils/basic.inc";

$w = new MongoDB\Driver\WriteConcern(MongoDB\Driver\WriteConcern::MAJORITY);
$w2 = new MongoDB\Driver\WriteConcern(MongoDB\Driver\WriteConcern::MAJORITY, 1000);

$w3 = new MongoDB\Driver\WriteConcern(2);
$w4 = new MongoDB\Driver\WriteConcern(2, 2000);

$w5 = new MongoDB\Driver\WriteConcern("tagname");
$w6 = new MongoDB\Driver\WriteConcern("string", 3000);

$w7 = new MongoDB\Driver\WriteConcern("string", 3000, false, false);
$w8 = new MongoDB\Driver\WriteConcern("string", 3000, true, true);

$w9 = new MongoDB\Driver\WriteConcern("string", 3000, true, false);
$w10= new MongoDB\Driver\WriteConcern("string", 3000, false, true);

var_dump($w, $w2, $w3, $w4, $w5, $w6, $w7, $w8, $w9, $w10);

try {
    new MongoDB\Driver\WriteConcern("string", 3000, false, true, 1);
} catch(InvalidArgumentException $e) {
    echo $e->getMessage(), "\n";
}

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
object(MongoDB\Driver\WriteConcern)#%d (%d) {
  ["w"]=>
  string(8) "majority"
  ["wmajority"]=>
  bool(true)
  ["wtimeout"]=>
  int(0)
  ["fsync"]=>
  bool(false)
  ["journal"]=>
  bool(false)
}
object(MongoDB\Driver\WriteConcern)#%d (%d) {
  ["w"]=>
  string(8) "majority"
  ["wmajority"]=>
  bool(true)
  ["wtimeout"]=>
  int(1000)
  ["fsync"]=>
  bool(false)
  ["journal"]=>
  bool(false)
}
object(MongoDB\Driver\WriteConcern)#%d (%d) {
  ["w"]=>
  int(2)
  ["wmajority"]=>
  bool(false)
  ["wtimeout"]=>
  int(0)
  ["fsync"]=>
  bool(false)
  ["journal"]=>
  bool(false)
}
object(MongoDB\Driver\WriteConcern)#%d (%d) {
  ["w"]=>
  int(2)
  ["wmajority"]=>
  bool(false)
  ["wtimeout"]=>
  int(2000)
  ["fsync"]=>
  bool(false)
  ["journal"]=>
  bool(false)
}
object(MongoDB\Driver\WriteConcern)#%d (%d) {
  ["w"]=>
  string(7) "tagname"
  ["wmajority"]=>
  bool(false)
  ["wtimeout"]=>
  int(0)
  ["fsync"]=>
  bool(false)
  ["journal"]=>
  bool(false)
}
object(MongoDB\Driver\WriteConcern)#%d (%d) {
  ["w"]=>
  string(6) "string"
  ["wmajority"]=>
  bool(false)
  ["wtimeout"]=>
  int(3000)
  ["fsync"]=>
  bool(false)
  ["journal"]=>
  bool(false)
}
object(MongoDB\Driver\WriteConcern)#%d (%d) {
  ["w"]=>
  string(6) "string"
  ["wmajority"]=>
  bool(false)
  ["wtimeout"]=>
  int(3000)
  ["fsync"]=>
  bool(false)
  ["journal"]=>
  bool(false)
}
object(MongoDB\Driver\WriteConcern)#%d (%d) {
  ["w"]=>
  string(6) "string"
  ["wmajority"]=>
  bool(false)
  ["wtimeout"]=>
  int(3000)
  ["fsync"]=>
  bool(true)
  ["journal"]=>
  bool(true)
}
object(MongoDB\Driver\WriteConcern)#%d (%d) {
  ["w"]=>
  string(6) "string"
  ["wmajority"]=>
  bool(false)
  ["wtimeout"]=>
  int(3000)
  ["fsync"]=>
  bool(false)
  ["journal"]=>
  bool(true)
}
object(MongoDB\Driver\WriteConcern)#%d (%d) {
  ["w"]=>
  string(6) "string"
  ["wmajority"]=>
  bool(false)
  ["wtimeout"]=>
  int(3000)
  ["fsync"]=>
  bool(true)
  ["journal"]=>
  bool(false)
}
MongoDB\Driver\WriteConcern::__construct() expects at most 4 parameters, 5 given
===DONE===
