--TEST--
MongoDB\BSON\Int64 unserialization requires "int" string field (__serialize and __unserialize)
--FILE--
<?php

require_once __DIR__ . '/../utils/basic.inc';

echo throws(function() {
    unserialize('O:18:"MongoDB\BSON\Int64":1:{s:7:"integer";i:0;}');
}, 'MongoDB\Driver\Exception\InvalidArgumentException'), "\n";

?>
===DONE===
<?php exit(0); ?>
--EXPECT--
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
MongoDB\BSON\Int64 initialization requires "integer" string field
===DONE===
