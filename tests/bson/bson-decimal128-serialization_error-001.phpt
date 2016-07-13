--TEST--
MongoDB\BSON\Decimal128 unserialization requires "dec" string field
--SKIPIF--
<?php require __DIR__ . '/../utils/basic-skipif.inc'?>
--FILE--
<?php
require_once __DIR__ . '/../utils/basic.inc';

echo throws(function() {
    unserialize('O:23:"MongoDB\BSON\Decimal128":1:{s:3:"dec";i:0;}');
}, 'MongoDB\Driver\Exception\InvalidArgumentException'), "\n";

?>
===DONE===
<?php exit(0); ?>
--EXPECT--
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
MongoDB\BSON\Decimal128 initialization requires "dec" string field
===DONE===
