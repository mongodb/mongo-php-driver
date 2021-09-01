--TEST--
MongoDB\Driver\ReadConcern unserialization errors (Serializable interface)
--FILE--
<?php

require_once __DIR__ . '/../utils/basic.inc';

echo throws(function() {
    unserialize('C:26:"MongoDB\Driver\ReadConcern":22:{a:1:{s:5:"level";i:0;}}');
}, 'MongoDB\Driver\Exception\InvalidArgumentException'), "\n";

?>
===DONE===
<?php exit(0); ?>
--EXPECT--
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
MongoDB\Driver\ReadConcern initialization requires "level" string field
===DONE===