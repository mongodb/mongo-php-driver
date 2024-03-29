--TEST--
MongoDB\Driver\ReadPreference::__set_state() requires correct data types and values
--FILE--
<?php

require_once __DIR__ . '/../utils/basic.inc';

echo throws(function() {
    MongoDB\Driver\ReadPreference::__set_state(['mode' => 'furthest']);
}, MongoDB\Driver\Exception\InvalidArgumentException::class), "\n";

echo throws(function() {
    MongoDB\Driver\ReadPreference::__set_state(['mode' => M_PI]);
}, MongoDB\Driver\Exception\InvalidArgumentException::class), "\n";


echo throws(function() {
    MongoDB\Driver\ReadPreference::__set_state(['mode' => 'secondary', 'tags' => -1]);
}, MongoDB\Driver\Exception\InvalidArgumentException::class), "\n";

echo throws(function() {
    MongoDB\Driver\ReadPreference::__set_state(['mode' => 'secondary', 'tags' => [ 42 ] ]);
}, MongoDB\Driver\Exception\InvalidArgumentException::class), "\n";

echo throws(function() {
    MongoDB\Driver\ReadPreference::__set_state(['mode' => 'primary', 'tags' => [['dc' => 'ny']]]);
}, MongoDB\Driver\Exception\InvalidArgumentException::class), "\n";


echo throws(function() {
    MongoDB\Driver\ReadPreference::__set_state(['mode' => 'secondary', 'maxStalenessSeconds' => 1]);
}, MongoDB\Driver\Exception\InvalidArgumentException::class), "\n";

echo throws(function() {
    MongoDB\Driver\ReadPreference::__set_state(['mode' => 'primary', 'maxStalenessSeconds' => 100]);
}, MongoDB\Driver\Exception\InvalidArgumentException::class), "\n";


echo throws(function() {
    MongoDB\Driver\ReadPreference::__set_state(['mode' => 'secondary', 'hedge' => 'foo']);
}, MongoDB\Driver\Exception\InvalidArgumentException::class), "\n";

echo throws(function() {
    MongoDB\Driver\ReadPreference::__set_state(['mode' => 'primary', 'hedge' => []]);
}, MongoDB\Driver\Exception\InvalidArgumentException::class), "\n";

echo throws(function() {
    MongoDB\Driver\ReadPreference::__set_state(['mode' => 'primaryPreferred', 'hedge' => MongoDB\BSON\PackedArray::fromPHP([])]);
}, MongoDB\Driver\Exception\UnexpectedValueException::class), "\n";

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
MongoDB\Driver\ReadPreference initialization requires "maxStalenessSeconds" field to not be present with "primary" mode
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
MongoDB\Driver\ReadPreference initialization requires "hedge" field to be an array or object
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
MongoDB\Driver\ReadPreference initialization requires "hedge" field to not be present with "primary" mode
OK: Got MongoDB\Driver\Exception\UnexpectedValueException
MongoDB\BSON\PackedArray cannot be serialized as a root document
===DONE===
