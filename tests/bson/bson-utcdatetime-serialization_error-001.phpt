--TEST--
MongoDB\BSON\UTCDateTime unserialization requires "milliseconds" integer or numeric string field
--SKIPIF--
<?php require __DIR__ . "/../utils/basic-skipif.inc"?>
--FILE--
<?php
require_once __DIR__ . "/../utils/basic.inc";

echo throws(function() {
    unserialize('O:24:"MongoDB\BSON\UTCDateTime":1:{s:12:"milliseconds";d:1;}');
}, 'MongoDB\Driver\Exception\InvalidArgumentException'), "\n";

?>
===DONE===
<?php exit(0); ?>
--EXPECT--
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
MongoDB\BSON\UTCDateTime initialization requires "milliseconds" integer or numeric string field
===DONE===
