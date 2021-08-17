--TEST--
MongoDB\BSON\Timestamp unserialization requires 64-bit integers to be positive unsigned 32-bit integers (__serialize and __unserialize)
--SKIPIF--
<?php require __DIR__ . "/../utils/basic-skipif.inc"; ?>
<?php if (8 !== PHP_INT_SIZE) { die('skip Only for 64-bit platform'); } ?>
<?php skip_if_php_version('<', '7.4.0'); ?>
--FILE--
<?php

require_once __DIR__ . '/../utils/basic.inc';

echo throws(function() {
    unserialize('O:22:"MongoDB\BSON\Timestamp":2:{s:9:"increment";i:4294967296;s:9:"timestamp";i:5678;}');
}, 'MongoDB\Driver\Exception\InvalidArgumentException'), "\n";

echo throws(function() {
    unserialize('O:22:"MongoDB\BSON\Timestamp":2:{s:9:"increment";i:1234;s:9:"timestamp";i:4294967296;}');
}, 'MongoDB\Driver\Exception\InvalidArgumentException'), "\n";

?>
===DONE===
<?php exit(0); ?>
--EXPECT--
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Expected increment to be an unsigned 32-bit integer, 4294967296 given
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Expected timestamp to be an unsigned 32-bit integer, 4294967296 given
===DONE===
