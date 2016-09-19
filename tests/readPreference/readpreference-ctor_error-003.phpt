--TEST--
MongoDB\Driver\ReadPreference construction (invalid maxStalenessMS)
--FILE--
<?php

require_once __DIR__ . '/../utils/tools.php';

echo throws(function() {
    new MongoDB\Driver\ReadPreference(MongoDB\Driver\ReadPreference::RP_PRIMARY, null, ['maxStalenessMS' => 1000]);
}, 'MongoDB\Driver\Exception\InvalidArgumentException'), "\n";

echo throws(function() {
    new MongoDB\Driver\ReadPreference(MongoDB\Driver\ReadPreference::RP_SECONDARY, null, ['maxStalenessMS' => -1]);
}, 'MongoDB\Driver\Exception\InvalidArgumentException'), "\n";

?>
===DONE===
<?php exit(0); ?>
--EXPECT--
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
maxStalenessMS may not be used with primary mode
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Expected maxStalenessMS to be >= 0, -1 given
===DONE===
