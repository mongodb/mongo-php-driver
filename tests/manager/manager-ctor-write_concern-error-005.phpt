--TEST--
MongoDB\Driver\Manager::__construct(): invalid write concern (journal)
--FILE--
<?php

require_once __DIR__ . '/../utils/tools.php';

// Invalid types

echo throws(function() {
    new MongoDB\Driver\Manager('mongodb://127.0.0.1/?journal=invalid');
}, "MongoDB\Driver\Exception\InvalidArgumentException"), "\n";

echo throws(function() {
    new MongoDB\Driver\Manager(null, ['journal' => 'invalid']);
}, "MongoDB\Driver\Exception\InvalidArgumentException"), "\n";

// Invalid values (journal conflicts with unacknowledged write concerns)

echo throws(function() {
    new MongoDB\Driver\Manager('mongodb://127.0.0.1/?w=-1&journal=true');
}, "MongoDB\Driver\Exception\InvalidArgumentException"), "\n";

echo throws(function() {
    new MongoDB\Driver\Manager('mongodb://127.0.0.1/?w=0&journal=true');
}, "MongoDB\Driver\Exception\InvalidArgumentException"), "\n";

echo throws(function() {
    new MongoDB\Driver\Manager('mongodb://127.0.0.1/?w=-1', ['journal' => true]);
}, "MongoDB\Driver\Exception\InvalidArgumentException"), "\n";

echo throws(function() {
    new MongoDB\Driver\Manager('mongodb://127.0.0.1/?w=0', ['journal' => true]);
}, "MongoDB\Driver\Exception\InvalidArgumentException"), "\n";

echo throws(function() {
    new MongoDB\Driver\Manager('mongodb://127.0.0.1/?journal=true', ['w' => -1]);
}, "MongoDB\Driver\Exception\InvalidArgumentException"), "\n";

echo throws(function() {
    new MongoDB\Driver\Manager('mongodb://127.0.0.1/?journal=true', ['w' => 0]);
}, "MongoDB\Driver\Exception\InvalidArgumentException"), "\n";

echo throws(function() {
    new MongoDB\Driver\Manager(null, ['w' => -1, 'journal' => true]);
}, "MongoDB\Driver\Exception\InvalidArgumentException"), "\n";

echo throws(function() {
    new MongoDB\Driver\Manager(null, ['w' => 0, 'journal' => true]);
}, "MongoDB\Driver\Exception\InvalidArgumentException"), "\n";

?>
===DONE===
<?php exit(0); ?>
--EXPECT--
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Failed to parse MongoDB URI: 'mongodb://127.0.0.1/?journal=invalid'. Unsupported value for "journal": "invalid".
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Expected boolean for "journal" URI option, string given
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Failed to parse MongoDB URI: 'mongodb://127.0.0.1/?w=-1&journal=true'. Journal conflicts with w value [w=-1].
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Failed to parse MongoDB URI: 'mongodb://127.0.0.1/?w=0&journal=true'. Journal conflicts with w value [w=0].
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
===DONE===
