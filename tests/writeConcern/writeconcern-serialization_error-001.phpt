--TEST--
MongoDB\Driver\WriteConcern serialization errors
--FILE--
<?php

require_once __DIR__ . '/../utils/basic.inc';

echo throws(function() {
    unserialize('C:27:"MongoDB\Driver\WriteConcern":30:{a:2:{s:1:"w";i:0;s:1:"j";b:1;}}');
}, 'MongoDB\Driver\Exception\InvalidArgumentException'), "\n";

?>
===DONE===
<?php exit(0); ?>
--EXPECT--
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Cannot enable journaling when using w = 0
===DONE===
