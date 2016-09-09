--TEST--
MongoDB\BSON\Timestamp unserialization requires positive unsigned 32-bit integers
--FILE--
<?php

require_once __DIR__ . '/../utils/tools.php';

echo throws(function() {
    unserialize('C:22:"MongoDB\BSON\Timestamp":50:{a:2:{s:9:"increment";i:-1;s:9:"timestamp";i:5678;}}');
}, 'MongoDB\Driver\Exception\InvalidArgumentException'), "\n";

echo throws(function() {
    unserialize('C:22:"MongoDB\BSON\Timestamp":59:{a:2:{s:9:"increment";i:-2147483648;s:9:"timestamp";i:5678;}}');
}, 'MongoDB\Driver\Exception\InvalidArgumentException'), "\n";

echo throws(function() {
    unserialize('C:22:"MongoDB\BSON\Timestamp":50:{a:2:{s:9:"increment";i:1234;s:9:"timestamp";i:-1;}}');
}, 'MongoDB\Driver\Exception\InvalidArgumentException'), "\n";

echo throws(function() {
    unserialize('C:22:"MongoDB\BSON\Timestamp":59:{a:2:{s:9:"increment";i:1234;s:9:"timestamp";i:-2147483648;}}');
}, 'MongoDB\Driver\Exception\InvalidArgumentException'), "\n";

?>
===DONE===
<?php exit(0); ?>
--EXPECT--
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Expected increment to be an unsigned 32-bit integer, -1 given
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Expected increment to be an unsigned 32-bit integer, -2147483648 given
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Expected timestamp to be an unsigned 32-bit integer, -1 given
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Expected timestamp to be an unsigned 32-bit integer, -2147483648 given
===DONE===
