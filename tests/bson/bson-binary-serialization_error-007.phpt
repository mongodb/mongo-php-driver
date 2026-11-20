--TEST--
MongoDB\BSON\Binary unserialization requires valid vector data (__serialize and __unserialize)
--FILE--
<?php

require_once __DIR__ . '/../utils/basic.inc';

echo throws(function() {
    unserialize('O:19:"MongoDB\BSON\Binary":2:{s:4:"data";s:0:"";s:4:"type";i:9;}');
}, 'MongoDB\Driver\Exception\InvalidArgumentException'), "\n";

?>
===DONE===
<?php exit(0); ?>
--EXPECT--
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Binary vector data is invalid
===DONE===
