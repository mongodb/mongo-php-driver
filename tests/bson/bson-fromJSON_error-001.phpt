--TEST--
BSON\fromJSON(): invalid JSON
--SKIPIF--
<?php require __DIR__ . "/../utils/basic-skipif.inc"?>
--FILE--
<?php

require_once __DIR__ . "/../utils/basic.inc";

var_dump(fromJSON('foo'));

?>
===DONE===
<?php exit(0); ?>
--EXPECT--
NULL
===DONE===
