--TEST--
MongoDB\BSON\UTCDateTime unserialization requires "milliseconds" string to parse as 64-bit integer (__serialize and __unserialize)
--SKIPIF--
<?php require __DIR__ . "/../utils/basic-skipif.inc"; ?>
<?php skip_if_php_version('<', '7.4.0'); ?>
--FILE--
<?php

require_once __DIR__ . '/../utils/basic.inc';

echo throws(function() {
    unserialize('O:24:"MongoDB\BSON\UTCDateTime":1:{s:12:"milliseconds";s:9:"1234.5678";}');
}, 'MongoDB\Driver\Exception\InvalidArgumentException'), "\n";

/* TODO: Add tests for out-of-range values once CDRIVER-1377 is resolved */

?>
===DONE===
<?php exit(0); ?>
--EXPECT--
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Error parsing "1234.5678" as 64-bit integer for MongoDB\BSON\UTCDateTime initialization
===DONE===
