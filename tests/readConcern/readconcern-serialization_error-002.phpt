--TEST--
MongoDB\Driver\ReadConcern unserialization errors (__serialize and __unserialize)
--FILE--
<?php

require_once __DIR__ . '/../utils/basic.inc';

echo throws(function() {
    unserialize('O:26:"MongoDB\Driver\ReadConcern":1:{s:5:"level";i:0;}');
}, 'MongoDB\Driver\Exception\InvalidArgumentException'), "\n";

?>
===DONE===
<?php exit(0); ?>
--EXPECT--
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
MongoDB\Driver\ReadConcern initialization requires "level" string field
===DONE===