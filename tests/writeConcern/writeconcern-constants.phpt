--TEST--
MongoDB\Driver\WriteConcern constants
--SKIPIF--
<?php require __DIR__ . "/../utils/basic-skipif.inc"?>
--FILE--
<?php
require_once __DIR__ . "/../utils/basic.inc";

var_dump(MongoDB\Driver\WriteConcern::MAJORITY);

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
string(8) "majority"
===DONE===
