--TEST--
BSON BSON\UTCDateTime constructor defaults to current time
--SKIPIF--
<?php require __DIR__ . "/../utils/basic-skipif.inc"?>
--FILE--
<?php
require_once __DIR__ . "/../utils/basic.inc";

$tests = [
    new MongoDB\BSON\UTCDateTime,
    new MongoDB\BSON\UTCDateTime(null),
];

foreach ($tests as $test) {
    var_dump($test);
}

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
object(%SBSON\UTCDateTime)#%d (%d) {
  ["milliseconds"]=>
  int(%d)
}
object(%SBSON\UTCDateTime)#%d (%d) {
  ["milliseconds"]=>
  int(%d)
}
===DONE===
