--TEST--
MongoDB\BSON\Timestamp::__set_state() requires "increment" and "timestamp" integer fields
--SKIPIF--
<?php require __DIR__ . "/../utils/basic-skipif.inc"; ?>
--FILE--
<?php
require_once __DIR__ . "/../utils/basic.inc";

echo throws(function() {
    MongoDB\BSON\Timestamp::__set_state(['increment' => 1234]);
}, 'MongoDB\Driver\Exception\InvalidArgumentException'), "\n";

echo throws(function() {
    MongoDB\BSON\Timestamp::__set_state(['timestamp' => 5678]);
}, 'MongoDB\Driver\Exception\InvalidArgumentException'), "\n";

echo throws(function() {
    MongoDB\BSON\Timestamp::__set_state(['increment' => '1234', 'timestamp' => '5678']);
}, 'MongoDB\Driver\Exception\InvalidArgumentException'), "\n";

?>
===DONE===
<?php exit(0); ?>
--EXPECT--
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
MongoDB\BSON\Timestamp initialization requires "increment" and "timestamp" integer fields
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
MongoDB\BSON\Timestamp initialization requires "increment" and "timestamp" integer fields
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
MongoDB\BSON\Timestamp initialization requires "increment" and "timestamp" integer fields
===DONE===
