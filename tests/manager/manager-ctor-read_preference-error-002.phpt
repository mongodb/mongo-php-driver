--TEST--
MongoDB\Driver\Manager::__construct(): invalid read preference (maxStalenessSeconds)
--FILE--
<?php

require_once __DIR__ . '/../utils/tools.php';

// Invalid types

echo throws(function() {
    new MongoDB\Driver\Manager('mongodb://127.0.0.1/?readPreference=secondary&maxStalenessSeconds=invalid');
}, "MongoDB\Driver\Exception\InvalidArgumentException"), "\n";

echo throws(function() {
    new MongoDB\Driver\Manager(null, ['maxStalenessSeconds' => 'invalid']);
}, "MongoDB\Driver\Exception\InvalidArgumentException"), "\n";

// Invalid range in URI string (array option is tested in 64-bit error test)

echo throws(function() {
    new MongoDB\Driver\Manager('mongodb://127.0.0.1/?readPreference=secondary&maxStalenessSeconds=2147483648');
}, "MongoDB\Driver\Exception\InvalidArgumentException"), "\n";

// Invalid values

echo throws(function() {
    new MongoDB\Driver\Manager('mongodb://127.0.0.1/?maxstalenessseconds=1231');
}, "MongoDB\Driver\Exception\InvalidArgumentException"), "\n";

echo throws(function() {
    new MongoDB\Driver\Manager('mongodb://127.0.0.1/?maxStalenessSeconds=1231');
}, "MongoDB\Driver\Exception\InvalidArgumentException"), "\n";

echo throws(function() {
    new MongoDB\Driver\Manager(null, ['maxstalenessseconds' => 1231]);
}, "MongoDB\Driver\Exception\InvalidArgumentException"), "\n";

echo throws(function() {
    new MongoDB\Driver\Manager(null, ['maxStalenessSeconds' => 1231]);
}, "MongoDB\Driver\Exception\InvalidArgumentException"), "\n";

echo throws(function() {
    new MongoDB\Driver\Manager(null, ['readPreference' => 'secondary', 'maxStalenessSeconds' => -2]);
}, "MongoDB\Driver\Exception\InvalidArgumentException"), "\n";

echo throws(function() {
    new MongoDB\Driver\Manager(null, ['readPreference' => 'secondary', 'maxStalenessSeconds' => 0]);
}, "MongoDB\Driver\Exception\InvalidArgumentException"), "\n";

echo throws(function() {
    new MongoDB\Driver\Manager(null, ['readPreference' => 'secondary', 'maxStalenessSeconds' => 42]);
}, "MongoDB\Driver\Exception\InvalidArgumentException"), "\n";

?>
===DONE===
<?php exit(0); ?>
--EXPECT--
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Failed to parse MongoDB URI: 'mongodb://127.0.0.1/?readPreference=secondary&maxStalenessSeconds=invalid'. Unsupported value for "maxstalenessseconds": "invalid".
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Expected integer for "maxStalenessSeconds" URI option, string given
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Failed to parse MongoDB URI: 'mongodb://127.0.0.1/?readPreference=secondary&maxStalenessSeconds=2147483648'. Unsupported value for "maxstalenessseconds": "2147483648".
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Failed to parse MongoDB URI: 'mongodb://127.0.0.1/?maxstalenessseconds=1231'. Invalid readPreferences.
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Failed to parse MongoDB URI: 'mongodb://127.0.0.1/?maxStalenessSeconds=1231'. Invalid readPreferences.
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
