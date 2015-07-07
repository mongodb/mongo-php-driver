--TEST--
Test for PHPC-325: Memory leak decoding buffers with multiple documents
--SKIPIF--
<?php require __DIR__ . "/../utils/basic-skipif.inc"?>
--FILE--
<?php
require_once __DIR__ . "/../utils/basic.inc";

$bson1 = fromJSON('{"x": "y"}');
$bson2 = fromJSON('{"a": "b"}');
$value = toArray($bson1 . $bson2);

var_dump($value);

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
Warning: %s\toArray(): Reading document did not exhaust input buffer in %s on line %d
NULL
===DONE===
