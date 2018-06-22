--TEST--
MongoDB\BSON\Int64 unserialization requires "int" string field
--FILE--
<?php

require_once __DIR__ . '/../utils/tools.php';

echo throws(function() {
    unserialize('C:18:"MongoDB\BSON\Int64":24:{a:1:{s:7:"integer";i:0;}}');
}, 'MongoDB\Driver\Exception\InvalidArgumentException'), "\n";

?>
===DONE===
<?php exit(0); ?>
--EXPECT--
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
MongoDB\BSON\Int64 initialization requires "integer" string field
===DONE===
