--TEST--
MongoDB\BSON\UTCDateTime::__set_state() (64-bit)
--SKIPIF--
<?php require __DIR__ . "/../utils/basic-skipif.inc"?>
--FILE--
<?php
require_once __DIR__ . "/../utils/basic.inc";

$tests = [
    0,
    -1416445411987,
    1416445411987,
];

foreach ($tests as $milliseconds) {
    var_export(MongoDB\BSON\UTCDateTime::__set_state([
        'milliseconds' => $milliseconds,
    ]));
    echo "\n\n";
}

?>
===DONE===
<?php exit(0); ?>
--EXPECT--
MongoDB\BSON\UTCDateTime::__set_state(array(
   'milliseconds' => 0,
))

MongoDB\BSON\UTCDateTime::__set_state(array(
   'milliseconds' => -1416445411987,
))

MongoDB\BSON\UTCDateTime::__set_state(array(
   'milliseconds' => 1416445411987,
))

===DONE===
