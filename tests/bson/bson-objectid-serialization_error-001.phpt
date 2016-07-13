--TEST--
MongoDB\BSON\ObjectID unserialization requires "oid" string field
--SKIPIF--
<?php require __DIR__ . "/../utils/basic-skipif.inc"?>
--FILE--
<?php
require_once __DIR__ . "/../utils/basic.inc";

echo throws(function() {
    unserialize('O:21:"MongoDB\BSON\ObjectID":1:{s:3:"oid";i:0;}');
}, 'MongoDB\Driver\Exception\InvalidArgumentException'), "\n";

?>
===DONE===
<?php exit(0); ?>
--EXPECT--
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
MongoDB\BSON\ObjectID initialization requires "oid" string field
===DONE===
