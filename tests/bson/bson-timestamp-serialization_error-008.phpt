--TEST--
MongoDB\BSON\Timestamp unserialization requires strings to parse as 64-bit integers (__serialize and __unserialize)
--SKIPIF--
<?php require __DIR__ . "/../utils/basic-skipif.inc"; ?>
<?php skip_if_php_version('<', '7.4.0'); ?>
--FILE--
<?php

require_once __DIR__ . '/../utils/basic.inc';

echo throws(function() {
    unserialize('O:22:"MongoDB\BSON\Timestamp":2:{s:9:"increment";s:4:"1.23";s:9:"timestamp";s:4:"5678";}');
}, 'MongoDB\Driver\Exception\InvalidArgumentException'), "\n";

echo throws(function() {
    unserialize('O:22:"MongoDB\BSON\Timestamp":2:{s:9:"increment";s:4:"1234";s:9:"timestamp";s:4:"5.67";}');
}, 'MongoDB\Driver\Exception\InvalidArgumentException'), "\n";

?>
===DONE===
<?php exit(0); ?>
--EXPECT--
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Error parsing "1.23" as 64-bit integer increment for MongoDB\BSON\Timestamp initialization
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Error parsing "5.67" as 64-bit integer timestamp for MongoDB\BSON\Timestamp initialization
===DONE===
