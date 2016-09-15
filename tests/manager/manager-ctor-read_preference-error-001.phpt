--TEST--
MongoDB\Driver\Manager::__construct(): invalid read preference
--FILE--
<?php

require_once __DIR__ . '/../utils/tools.php';

echo throws(function() {
    $manager = new MongoDB\Driver\Manager('mongodb://127.0.0.1/?readPreference=primary&readPreferenceTags=dc:ny');
}, "MongoDB\Driver\Exception\InvalidArgumentException"), "\n";

echo throws(function() {
    $manager = new MongoDB\Driver\Manager(null, ['readPreference' => 'nothing']);
}, "MongoDB\Driver\Exception\InvalidArgumentException"), "\n";

echo throws(function() {
    $manager = new MongoDB\Driver\Manager('mongodb://127.0.0.1/?readPreference=primary', ['readPreferenceTags' => [[]]]);
}, "MongoDB\Driver\Exception\InvalidArgumentException"), "\n";

echo throws(function() {
    $manager = new MongoDB\Driver\Manager('mongodb://127.0.0.1/?readPreference=primary', ['readPreferenceTags' => ['invalid']]);
}, "MongoDB\Driver\Exception\InvalidArgumentException"), "\n";

?>
===DONE===
<?php exit(0); ?>
--EXPECT--
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Failed to parse MongoDB URI: 'mongodb://127.0.0.1/?readPreference=primary&readPreferenceTags=dc:ny'
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Unsupported readPreference value: 'nothing'
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Primary read preference mode conflicts with tags
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Read preference tags must be an array of zero or more documents
===DONE===
