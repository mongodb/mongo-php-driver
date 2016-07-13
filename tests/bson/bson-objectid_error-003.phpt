--TEST--
MongoDB\BSON\ObjectID::__construct() requires valid hex string
--SKIPIF--
<?php require __DIR__ . "/../utils/basic-skipif.inc"?>
--FILE--
<?php
require_once __DIR__ . "/../utils/basic.inc";

echo throws(function() {
    new MongoDB\BSON\ObjectID('0123456789abcdefghijklmn');
}, 'MongoDB\Driver\Exception\InvalidArgumentException'), "\n";

echo throws(function() {
    new MongoDB\BSON\ObjectID('INVALID');
}, 'MongoDB\Driver\Exception\InvalidArgumentException'), "\n";

?>
===DONE===
<?php exit(0); ?>
--EXPECT--
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Invalid BSON ID provided
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Invalid BSON ID provided
===DONE===
