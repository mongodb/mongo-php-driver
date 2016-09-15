--TEST--
MongoDB\Driver\Manager::__construct(): invalid write concern
--FILE--
<?php

require_once __DIR__ . '/../utils/tools.php';

echo throws(function() {
    $manager = new MongoDB\Driver\Manager('mongodb://127.0.0.1/?w=-1&journal=true');
}, "MongoDB\Driver\Exception\InvalidArgumentException"), "\n";

echo throws(function() {
    $manager = new MongoDB\Driver\Manager('mongodb://127.0.0.1/?w=0&journal=true');
}, "MongoDB\Driver\Exception\InvalidArgumentException"), "\n";

echo throws(function() {
    $manager = new MongoDB\Driver\Manager('mongodb://127.0.0.1/?w=-1', ['journal' => true]);
}, "MongoDB\Driver\Exception\InvalidArgumentException"), "\n";

echo throws(function() {
    $manager = new MongoDB\Driver\Manager('mongodb://127.0.0.1/?w=0', ['journal' => true]);
}, "MongoDB\Driver\Exception\InvalidArgumentException"), "\n";

echo throws(function() {
    $manager = new MongoDB\Driver\Manager('mongodb://127.0.0.1/?journal=true', ['w' => -1]);
}, "MongoDB\Driver\Exception\InvalidArgumentException"), "\n";

echo throws(function() {
    $manager = new MongoDB\Driver\Manager('mongodb://127.0.0.1/?journal=true', ['w' => 0]);
}, "MongoDB\Driver\Exception\InvalidArgumentException"), "\n";

echo throws(function() {
    $manager = new MongoDB\Driver\Manager(null, ['w' => -1, 'journal' => true]);
}, "MongoDB\Driver\Exception\InvalidArgumentException"), "\n";

echo throws(function() {
    $manager = new MongoDB\Driver\Manager(null, ['w' => 0, 'journal' => true]);
}, "MongoDB\Driver\Exception\InvalidArgumentException"), "\n";

echo throws(function() {
    $manager = new MongoDB\Driver\Manager(null, ['w' => -2]);
}, "MongoDB\Driver\Exception\InvalidArgumentException"), "\n";

?>
===DONE===
<?php exit(0); ?>
--EXPECT--
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Failed to parse MongoDB URI: 'mongodb://127.0.0.1/?w=-1&journal=true'
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Failed to parse MongoDB URI: 'mongodb://127.0.0.1/?w=0&journal=true'
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
