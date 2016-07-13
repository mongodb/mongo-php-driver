--TEST--
MongoDB\BSON\ObjectID::__set_state() requires "oid" string field
--SKIPIF--
<?php require __DIR__ . "/../utils/basic-skipif.inc"?>
--FILE--
<?php
require_once __DIR__ . "/../utils/basic.inc";

echo throws(function() {
    MongoDB\BSON\ObjectID::__set_state(['oid' => 0]);
}, 'MongoDB\Driver\Exception\InvalidArgumentException'), "\n";

?>
===DONE===
<?php exit(0); ?>
--EXPECT--
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
MongoDB\BSON\ObjectID initialization requires "oid" string field
===DONE===
