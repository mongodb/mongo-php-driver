--TEST--
MongoDB\Driver\ReadPreference construction (invalid tagSets)
--FILE--
<?php

require_once __DIR__ . '/../utils/tools.php';

echo throws(function() {
    new MongoDB\Driver\ReadPreference(MongoDB\Driver\ReadPreference::RP_PRIMARY, [['tag' => 'one']]);
}, 'MongoDB\Driver\Exception\InvalidArgumentException'), "\n";

echo throws(function() {
    new MongoDB\Driver\ReadPreference("primary", [['tag' => 'one']]);
}, 'MongoDB\Driver\Exception\InvalidArgumentException'), "\n";

echo throws(function() {
    new MongoDB\Driver\ReadPreference(MongoDB\Driver\ReadPreference::RP_PRIMARY, ['invalid']);
}, 'MongoDB\Driver\Exception\InvalidArgumentException'), "\n";

echo throws(function() {
    new MongoDB\Driver\ReadPreference(MongoDB\Driver\ReadPreference::RP_SECONDARY, ['invalid']);
}, 'MongoDB\Driver\Exception\InvalidArgumentException'), "\n";

// Ensure that tagSets is validated before maxStalenessSeconds option
echo throws(function() {
    new MongoDB\Driver\ReadPreference(MongoDB\Driver\ReadPreference::RP_SECONDARY, ['invalid'], ['maxStalenessSeconds' => -2]);
}, 'MongoDB\Driver\Exception\InvalidArgumentException'), "\n";

?>
===DONE===
<?php exit(0); ?>
--EXPECT--
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
tagSets may not be used with primary mode
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
tagSets may not be used with primary mode
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
tagSets must be an array of zero or more documents
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
tagSets must be an array of zero or more documents
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
tagSets must be an array of zero or more documents
===DONE===
