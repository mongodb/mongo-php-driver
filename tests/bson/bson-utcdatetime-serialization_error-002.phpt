--TEST--
MongoDB\BSON\UTCDateTime unserialization requires "milliseconds" string to parse as 64-bit integer (Serializable interface)
--FILE--
<?php

require_once __DIR__ . '/../utils/basic.inc';

echo throws(function() {
    unserialize('C:24:"MongoDB\BSON\UTCDateTime":42:{a:1:{s:12:"milliseconds";s:9:"1234.5678";}}');
}, 'MongoDB\Driver\Exception\InvalidArgumentException'), "\n";

/* TODO: Add tests for out-of-range values once CDRIVER-1377 is resolved */

?>
===DONE===
<?php exit(0); ?>
--EXPECT--
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Error parsing "1234.5678" as 64-bit integer for MongoDB\BSON\UTCDateTime initialization
===DONE===
