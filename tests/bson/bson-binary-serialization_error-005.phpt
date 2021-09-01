--TEST--
MongoDB\BSON\Binary unserialization requires unsigned 8-bit integer for type (__serialize and __unserialize)
--SKIPIF--
<?php require __DIR__ . "/../utils/basic-skipif.inc"; ?>
<?php skip_if_php_version('<', '7.4.0'); ?>
--FILE--
<?php

require_once __DIR__ . '/../utils/basic.inc';

echo throws(function() {
    unserialize('O:19:"MongoDB\BSON\Binary":2:{s:4:"data";s:6:"foobar";s:4:"type";i:-1;}');
}, 'MongoDB\Driver\Exception\InvalidArgumentException'), "\n";

echo throws(function() {
    unserialize('O:19:"MongoDB\BSON\Binary":2:{s:4:"data";s:6:"foobar";s:4:"type";i:256;}');
}, 'MongoDB\Driver\Exception\InvalidArgumentException'), "\n";

?>
===DONE===
<?php exit(0); ?>
--EXPECT--
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Expected type to be an unsigned 8-bit integer, -1 given
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Expected type to be an unsigned 8-bit integer, 256 given
===DONE===
