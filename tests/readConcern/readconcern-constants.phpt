--TEST--
MongoDB\Driver\ReadConcern constants
--SKIPIF--
<?php require __DIR__ . "/../utils/basic-skipif.inc"?>
--FILE--
<?php
require_once __DIR__ . "/../utils/basic.inc";

var_dump(MongoDB\Driver\ReadConcern::LOCAL);
var_dump(MongoDB\Driver\ReadConcern::MAJORITY);

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
string(5) "local"
string(8) "majority"
===DONE===
