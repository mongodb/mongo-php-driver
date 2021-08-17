--TEST--
MongoDB\BSON\Timestamp unserialization requires positive unsigned 32-bit integers (__serialize and __unserialize)
--SKIPIF--
<?php require __DIR__ . "/../utils/basic-skipif.inc"; ?>
<?php skip_if_php_version('<', '7.4.0'); ?>
--FILE--
<?php

require_once __DIR__ . '/../utils/basic.inc';

echo throws(function() {
    unserialize('O:22:"MongoDB\BSON\Timestamp":2:{s:9:"increment";i:-1;s:9:"timestamp";i:5678;}');
}, 'MongoDB\Driver\Exception\InvalidArgumentException'), "\n";

echo throws(function() {
    unserialize('O:22:"MongoDB\BSON\Timestamp":2:{s:9:"increment";i:-2147483648;s:9:"timestamp";i:5678;}');
}, 'MongoDB\Driver\Exception\InvalidArgumentException'), "\n";

echo throws(function() {
    unserialize('O:22:"MongoDB\BSON\Timestamp":2:{s:9:"increment";i:1234;s:9:"timestamp";i:-1;}');
}, 'MongoDB\Driver\Exception\InvalidArgumentException'), "\n";

echo throws(function() {
    unserialize('O:22:"MongoDB\BSON\Timestamp":2:{s:9:"increment";i:1234;s:9:"timestamp";i:-2147483648;}');
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
