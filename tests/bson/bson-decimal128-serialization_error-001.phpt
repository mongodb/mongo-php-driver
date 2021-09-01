--TEST--
MongoDB\BSON\Decimal128 unserialization requires "dec" string field (Serializable interface)
--FILE--
<?php

require_once __DIR__ . '/../utils/basic.inc';

echo throws(function() {
    unserialize('C:23:"MongoDB\BSON\Decimal128":20:{a:1:{s:3:"dec";i:0;}}');
}, 'MongoDB\Driver\Exception\InvalidArgumentException'), "\n";

?>
===DONE===
<?php exit(0); ?>
--EXPECT--
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
MongoDB\BSON\Decimal128 initialization requires "dec" string field
===DONE===
