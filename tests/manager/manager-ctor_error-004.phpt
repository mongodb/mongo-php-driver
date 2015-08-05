--TEST--
MongoDB\Driver\Manager::__construct(): invalid write concern
--SKIPIF--
<?php require __DIR__ . "/../utils/basic-skipif.inc"?>
--FILE--
<?php
require_once __DIR__ . "/../utils/basic.inc";

echo throws(function() {
    $manager = new MongoDB\Driver\Manager(STANDALONE . '/?w=-1&journal=true');
}, "MongoDB\Driver\Exception\InvalidArgumentException"), "\n";

echo throws(function() {
    $manager = new MongoDB\Driver\Manager(STANDALONE . '/?w=0&journal=true');
}, "MongoDB\Driver\Exception\InvalidArgumentException"), "\n";

echo throws(function() {
    $manager = new MongoDB\Driver\Manager(STANDALONE . '/?w=-1', array('journal' => true));
}, "MongoDB\Driver\Exception\InvalidArgumentException"), "\n";

echo throws(function() {
    $manager = new MongoDB\Driver\Manager(STANDALONE . '/?w=0', array('journal' => true));
}, "MongoDB\Driver\Exception\InvalidArgumentException"), "\n";

echo throws(function() {
    $manager = new MongoDB\Driver\Manager(STANDALONE . '/?journal=true', array('w' => -1));
}, "MongoDB\Driver\Exception\InvalidArgumentException"), "\n";

echo throws(function() {
    $manager = new MongoDB\Driver\Manager(STANDALONE . '/?journal=true', array('w' => 0));
}, "MongoDB\Driver\Exception\InvalidArgumentException"), "\n";

echo throws(function() {
    $manager = new MongoDB\Driver\Manager(STANDALONE, array('w' => -1, 'journal' => true));
}, "MongoDB\Driver\Exception\InvalidArgumentException"), "\n";

echo throws(function() {
    $manager = new MongoDB\Driver\Manager(STANDALONE, array('w' => 0, 'journal' => true));
}, "MongoDB\Driver\Exception\InvalidArgumentException"), "\n";

echo throws(function() {
    $manager = new MongoDB\Driver\Manager(STANDALONE, array('w' => -2));
}, "MongoDB\Driver\Exception\InvalidArgumentException"), "\n";

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Failed to parse MongoDB URI: '%s/?w=-1&journal=true'
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Failed to parse MongoDB URI: '%s/?w=0&journal=true'
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Journal conflicts with w value: -1
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Journal conflicts with w value: 0
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Journal conflicts with w value: -1
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Journal conflicts with w value: 0
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Journal conflicts with w value: -1
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Journal conflicts with w value: 0
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Unsupported w value: -2
===DONE===
