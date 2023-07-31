--TEST--
MongoDB\BSON\Int64 unserialization requires "int" string field to be valid (__serialize and __unserialize)
--FILE--
<?php

require_once __DIR__ . '/../utils/basic.inc';

echo throws(function() {
    unserialize('O:18:"MongoDB\BSON\Int64":1:{s:7:"integer";s:9:"1234.5678";}');
}, 'MongoDB\Driver\Exception\InvalidArgumentException'), "\n";

?>
===DONE===
<?php exit(0); ?>
--EXPECT--
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Error parsing "1234.5678" as 64-bit integer for MongoDB\BSON\Int64 initialization
===DONE===
