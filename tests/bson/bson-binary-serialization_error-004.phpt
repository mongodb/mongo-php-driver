--TEST--
MongoDB\BSON\Binary unserialization requires "data" string and "type" integer fields (__serialize and __unserialize)
--SKIPIF--
<?php require __DIR__ . "/../utils/basic-skipif.inc"; ?>
<?php skip_if_php_version('<', '7.4.0'); ?>
--FILE--
<?php

require_once __DIR__ . '/../utils/basic.inc';

echo throws(function() {
    unserialize('O:19:"MongoDB\BSON\Binary":1:{s:4:"data";s:6:"foobar";}');
}, 'MongoDB\Driver\Exception\InvalidArgumentException'), "\n";

echo throws(function() {
    unserialize('O:19:"MongoDB\BSON\Binary":1:{s:4:"type";i:0;}');
}, 'MongoDB\Driver\Exception\InvalidArgumentException'), "\n";

echo throws(function() {
    unserialize('O:19:"MongoDB\BSON\Binary":2:{s:4:"data";i:0;s:4:"type";s:6:"foobar";}');
}, 'MongoDB\Driver\Exception\InvalidArgumentException'), "\n";

?>
===DONE===
<?php exit(0); ?>
--EXPECT--
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
MongoDB\BSON\Binary initialization requires "data" string and "type" integer fields
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
MongoDB\BSON\Binary initialization requires "data" string and "type" integer fields
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
MongoDB\BSON\Binary initialization requires "data" string and "type" integer fields
===DONE===
