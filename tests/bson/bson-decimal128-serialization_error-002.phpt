--TEST--
MongoDB\BSON\Decimal128 unserialization requires valid decimal string
--SKIPIF--
<?php require __DIR__ . '/../utils/basic-skipif.inc'?>
--FILE--
<?php
require_once __DIR__ . '/../utils/basic.inc';

echo throws(function() {
    unserialize('O:23:"MongoDB\BSON\Decimal128":1:{s:3:"dec";s:7:"INVALID";}');
}, 'MongoDB\Driver\Exception\InvalidArgumentException'), "\n";

?>
===DONE===
<?php exit(0); ?>
--EXPECT--
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Error parsing decimal string: INVALID
===DONE===
