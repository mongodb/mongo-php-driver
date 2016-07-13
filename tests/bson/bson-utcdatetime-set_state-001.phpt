--TEST--
MongoDB\BSON\UTCDateTime::__set_state()
--SKIPIF--
<?php require __DIR__ . "/../utils/basic-skipif.inc"?>
--FILE--
<?php
require_once __DIR__ . "/../utils/basic.inc";

$tests = [
    '0',
    '-1416445411987',
    '1416445411987',
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
--EXPECTF--
MongoDB\BSON\UTCDateTime::__set_state(array(
   'milliseconds' => %r'?%r0%r'?%r,
))

MongoDB\BSON\UTCDateTime::__set_state(array(
   'milliseconds' => %r'?%r-1416445411987%r'?%r,
))

MongoDB\BSON\UTCDateTime::__set_state(array(
   'milliseconds' => %r'?%r1416445411987%r'?%r,
))

===DONE===
