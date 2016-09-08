--TEST--
MongoDB\BSON\ObjectID unserialization requires "oid" string field
--FILE--
<?php

require_once __DIR__ . '/../utils/tools.php';

echo throws(function() {
    unserialize('C:21:"MongoDB\BSON\ObjectID":20:{a:1:{s:3:"oid";i:0;}}');
}, 'MongoDB\Driver\Exception\InvalidArgumentException'), "\n";

?>
===DONE===
<?php exit(0); ?>
--EXPECT--
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
MongoDB\BSON\ObjectID initialization requires "oid" string field
===DONE===
