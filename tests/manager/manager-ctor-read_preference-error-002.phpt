--TEST--
MongoDB\Driver\Manager::__construct(): invalid read preference (maxStalenessSeconds)
--FILE--
<?php

require_once __DIR__ . '/../utils/tools.php';

echo throws(function() {
    $manager = new MongoDB\Driver\Manager('mongodb://127.0.0.1/?maxstalenessseconds=1231');
}, "MongoDB\Driver\Exception\InvalidArgumentException"), "\n";

echo throws(function() {
    $manager = new MongoDB\Driver\Manager('mongodb://127.0.0.1/?maxStalenessSeconds=1231');
}, "MongoDB\Driver\Exception\InvalidArgumentException"), "\n";

echo throws(function() {
    $manager = new MongoDB\Driver\Manager(null, ['maxstalenessseconds' => 1231]);
}, "MongoDB\Driver\Exception\InvalidArgumentException"), "\n";

echo throws(function() {
    $manager = new MongoDB\Driver\Manager(null, ['maxStalenessSeconds' => 1231]);
}, "MongoDB\Driver\Exception\InvalidArgumentException"), "\n";

echo throws(function() {
    $manager = new MongoDB\Driver\Manager(null, ['readPreference' => 'secondary', 'maxStalenessSeconds' => -2]);
}, "MongoDB\Driver\Exception\InvalidArgumentException"), "\n";

echo throws(function() {
    $manager = new MongoDB\Driver\Manager(null, ['readPreference' => 'secondary', 'maxStalenessSeconds' => 0]);
}, "MongoDB\Driver\Exception\InvalidArgumentException"), "\n";

echo throws(function() {
    $manager = new MongoDB\Driver\Manager(null, ['readPreference' => 'secondary', 'maxStalenessSeconds' => 42]);
}, "MongoDB\Driver\Exception\InvalidArgumentException"), "\n";

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Failed to parse MongoDB URI: 'mongodb://127.0.0.1/?maxstalenessseconds=1231'
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Failed to parse MongoDB URI: 'mongodb://127.0.0.1/?maxStalenessSeconds=1231'
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Primary read preference mode conflicts with maxStalenessSeconds
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Primary read preference mode conflicts with maxStalenessSeconds
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Expected maxStalenessSeconds to be >= 90, -2 given
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Expected maxStalenessSeconds to be >= 90, 0 given
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Expected maxStalenessSeconds to be >= 90, 42 given
===DONE===
