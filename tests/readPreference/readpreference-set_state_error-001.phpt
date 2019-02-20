--TEST--
MongoDB\Driver\ReadPreference::__set_state() requires correct data types and values
--FILE--
<?php

require_once __DIR__ . '/../utils/tools.php';

echo throws(function() {
    MongoDB\Driver\ReadPreference::__set_state(['mode' => 'furthest']);
}, 'MongoDB\Driver\Exception\InvalidArgumentException'), "\n";

echo throws(function() {
    MongoDB\Driver\ReadPreference::__set_state(['mode' => M_PI]);
}, 'MongoDB\Driver\Exception\InvalidArgumentException'), "\n";


echo throws(function() {
    MongoDB\Driver\ReadPreference::__set_state(['mode' => 'secondary', 'tags' => -1]);
}, 'MongoDB\Driver\Exception\InvalidArgumentException'), "\n";

echo throws(function() {
    MongoDB\Driver\ReadPreference::__set_state(['mode' => 'secondary', 'tags' => [ 42 ] ]);
}, 'MongoDB\Driver\Exception\InvalidArgumentException'), "\n";

echo throws(function() {
    MongoDB\Driver\ReadPreference::__set_state(['mode' => 'primary', 'tags' => [['dc' => 'ny']]]);
}, 'MongoDB\Driver\Exception\InvalidArgumentException'), "\n";


echo throws(function() {
    MongoDB\Driver\ReadPreference::__set_state(['mode' => 'secondary', 'maxStalenessSeconds' => 1]);
}, 'MongoDB\Driver\Exception\InvalidArgumentException'), "\n";

echo throws(function() {
    MongoDB\Driver\ReadPreference::__set_state(['mode' => 'primary', 'maxStalenessSeconds' => 100]);
}, 'MongoDB\Driver\Exception\InvalidArgumentException'), "\n";

?>
===DONE===
<?php exit(0); ?>
--EXPECT--
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
MongoDB\Driver\ReadPreference initialization requires specific values for "mode" string field
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
MongoDB\Driver\ReadPreference initialization requires "mode" field to be string
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
MongoDB\Driver\ReadPreference initialization requires "tags" field to be array
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
MongoDB\Driver\ReadPreference initialization requires "tags" array field to have zero or more documents
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
MongoDB\Driver\ReadPreference initialization requires "tags" array field to not be present with "primary" mode
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
MongoDB\Driver\ReadPreference initialization requires "maxStalenessSeconds" integer field to be >= 90
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
MongoDB\Driver\ReadPreference initialization requires "maxStalenessSeconds" array field to not be present with "primary" mode
===DONE===
