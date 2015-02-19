--TEST--
MongoDB\Driver\WriteConcern::MAJORITY
--SKIPIF--
<?php require "tests/utils/basic-skipif.inc"?>
--FILE--
<?php
require_once "tests/utils/basic.inc";

var_dump(MongoDB\Driver\WriteConcern::MAJORITY);

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
string(8) "majority"
===DONE===
