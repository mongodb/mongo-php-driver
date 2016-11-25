--TEST--
MongoDB\BSON\UTCDateTime constructor requires strings to parse as 64-bit integers
--FILE--
<?php

require_once __DIR__ . '/../utils/tools.php';

echo throws(function() {
    new MongoDB\BSON\UTCDateTime('1234.5678');
}, 'MongoDB\Driver\Exception\InvalidArgumentException'), "\n";

/* TODO: Add tests for out-of-range values once CDRIVER-1377 is resolved */

?>
===DONE===
<?php exit(0); ?>
--EXPECT--
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Error parsing "1234.5678" as 64-bit integer for MongoDB\BSON\UTCDateTime initialization
===DONE===
