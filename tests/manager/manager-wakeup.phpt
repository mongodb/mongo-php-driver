--TEST--
MongoDB\Driver\Manager: Manager cannot be woken up
--SKIPIF--
<?php require __DIR__ . "/../utils/basic-skipif.inc"?>
--FILE--
<?php
require_once __DIR__ . "/../utils/basic.inc";

$manager = new MongoDB\Driver\Manager(STANDALONE);
throws(function() use($manager) {
    $manager->__wakeUp();
}, "MongoDB\Driver\Exception\RuntimeException");

$manager->__wakeUp(1, 2);

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
OK: Got MongoDB\Driver\Exception\RuntimeException

Warning: MongoDB\Driver\Manager::__wakeUp() expects exactly 0 parameters, 2 given in %s on line %d
===DONE===
