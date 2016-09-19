--TEST--
MongoDB\Driver\Manager::__construct(): invalid read preference (maxStalenessMS)
--FILE--
<?php

require_once __DIR__ . '/../utils/tools.php';

echo throws(function() {
    $manager = new MongoDB\Driver\Manager('mongodb://127.0.0.1/?maxstalenessms=1231');
}, "MongoDB\Driver\Exception\InvalidArgumentException"), "\n";

echo throws(function() {
    $manager = new MongoDB\Driver\Manager('mongodb://127.0.0.1/?maxStalenessMS=1231');
}, "MongoDB\Driver\Exception\InvalidArgumentException"), "\n";

echo throws(function() {
    $manager = new MongoDB\Driver\Manager(null, ['maxstalenessms' => 1231]);
}, "MongoDB\Driver\Exception\InvalidArgumentException"), "\n";

echo throws(function() {
    $manager = new MongoDB\Driver\Manager(null, ['maxStalenessMS' => 1231]);
}, "MongoDB\Driver\Exception\InvalidArgumentException"), "\n";

echo throws(function() {
    $manager = new MongoDB\Driver\Manager(null, ['readPreference' => 'secondary', 'maxStalenessMS' => -1]);
}, "MongoDB\Driver\Exception\InvalidArgumentException"), "\n";

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Failed to parse MongoDB URI: 'mongodb://127.0.0.1/?maxstalenessms=1231'
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Failed to parse MongoDB URI: 'mongodb://127.0.0.1/?maxStalenessMS=1231'
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Primary read preference mode conflicts with maxStalenessMS
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Primary read preference mode conflicts with maxStalenessMS
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Expected maxStalenessMS to be >= 0, -1 given
===DONE===
