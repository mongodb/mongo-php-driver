--TEST--
MongoDB\BSON\UTCDateTime unserialization requires "milliseconds" string to parse as 64-bit integer
--SKIPIF--
<?php require __DIR__ . "/../utils/basic-skipif.inc"?>
--FILE--
<?php
require_once __DIR__ . "/../utils/basic.inc";

echo throws(function() {
    unserialize('O:24:"MongoDB\BSON\UTCDateTime":1:{s:12:"milliseconds";s:7:"INVALID";}');
}, 'MongoDB\Driver\Exception\InvalidArgumentException'), "\n";

/* TODO: Add tests for out-of-range values once CDRIVER-1377 is resolved */

?>
===DONE===
<?php exit(0); ?>
--EXPECT--
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Error parsing "INVALID" as 64-bit integer for MongoDB\BSON\UTCDateTime initialization
===DONE===
