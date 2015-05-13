--TEST--
MongoDB\Driver\Manager: Constructing invalid manager
--SKIPIF--
<?php require __DIR__ . "/../utils/basic-skipif.inc"?>
--FILE--
<?php
require_once __DIR__ . "/../utils/basic.inc";

throws(function() {
    $manager = new MongoDB\Driver\Manager(STANDALONE, array(), array(), 1);
}, "MongoDB\Driver\Exception\InvalidArgumentException");

throws(function() {
    $manager = new MongoDB\Driver\Manager("not a valid connection string");
}, "MongoDB\Driver\Exception\RuntimeException");

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
OK: Got MongoDB\Driver\Exception\RuntimeException
===DONE===
