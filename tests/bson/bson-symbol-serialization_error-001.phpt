--TEST--
MongoDB\BSON\Symbol unserialization requires "symbol" string field
--FILE--
<?php

require_once __DIR__ . '/../utils/tools.php';

echo throws(function() {
    unserialize('C:19:"MongoDB\BSON\Symbol":23:{a:1:{s:6:"symbol";i:0;}}');
}, 'MongoDB\Driver\Exception\InvalidArgumentException'), "\n";

?>
===DONE===
<?php exit(0); ?>
--EXPECT--
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
MongoDB\BSON\Symbol initialization requires "symbol" string field
===DONE===
