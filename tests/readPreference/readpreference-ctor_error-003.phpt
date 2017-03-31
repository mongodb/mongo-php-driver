--TEST--
MongoDB\Driver\ReadPreference construction (invalid maxStalenessSeconds)
--FILE--
<?php

require_once __DIR__ . '/../utils/tools.php';

echo throws(function() {
    new MongoDB\Driver\ReadPreference(MongoDB\Driver\ReadPreference::RP_PRIMARY, null, ['maxStalenessSeconds' => 1000]);
}, 'MongoDB\Driver\Exception\InvalidArgumentException'), "\n";

echo throws(function() {
    new MongoDB\Driver\ReadPreference("primary", null, ['maxStalenessSeconds' => 1000]);
}, 'MongoDB\Driver\Exception\InvalidArgumentException'), "\n";

echo throws(function() {
    new MongoDB\Driver\ReadPreference(MongoDB\Driver\ReadPreference::RP_SECONDARY, null, ['maxStalenessSeconds' => -2]);
}, 'MongoDB\Driver\Exception\InvalidArgumentException'), "\n";

echo throws(function() {
    new MongoDB\Driver\ReadPreference(MongoDB\Driver\ReadPreference::RP_SECONDARY, null, ['maxStalenessSeconds' => 0]);
}, 'MongoDB\Driver\Exception\InvalidArgumentException'), "\n";

echo throws(function() {
    new MongoDB\Driver\ReadPreference(MongoDB\Driver\ReadPreference::RP_SECONDARY, null, ['maxStalenessSeconds' => 42]);
}, 'MongoDB\Driver\Exception\InvalidArgumentException'), "\n";

?>
===DONE===
<?php exit(0); ?>
--EXPECT--
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
maxStalenessSeconds may not be used with primary mode
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
maxStalenessSeconds may not be used with primary mode
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Expected maxStalenessSeconds to be >= 90, -2 given
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Expected maxStalenessSeconds to be >= 90, 0 given
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Expected maxStalenessSeconds to be >= 90, 42 given
===DONE===
