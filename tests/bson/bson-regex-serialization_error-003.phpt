--TEST--
MongoDB\BSON\Regex unserialization requires "pattern" and "flags" string fields (__serialize and __unserialize)
--SKIPIF--
<?php require __DIR__ . "/../utils/basic-skipif.inc"; ?>
<?php skip_if_php_version('<', '7.4.0'); ?>
--FILE--
<?php

require_once __DIR__ . '/../utils/basic.inc';

echo throws(function() {
    unserialize('O:18:"MongoDB\BSON\Regex":1:{s:7:"pattern";s:6:"regexp";}');
}, 'MongoDB\Driver\Exception\InvalidArgumentException'), "\n";

echo throws(function() {
    unserialize('O:18:"MongoDB\BSON\Regex":1:{s:5:"flags";s:1:"i";}');
}, 'MongoDB\Driver\Exception\InvalidArgumentException'), "\n";

echo throws(function() {
    unserialize('O:18:"MongoDB\BSON\Regex":2:{s:7:"pattern";i:0;s:5:"flags";i:0;}');
}, 'MongoDB\Driver\Exception\InvalidArgumentException'), "\n";

?>
===DONE===
<?php exit(0); ?>
--EXPECT--
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
MongoDB\BSON\Regex initialization requires "pattern" and "flags" string fields
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
MongoDB\BSON\Regex initialization requires "pattern" and "flags" string fields
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
MongoDB\BSON\Regex initialization requires "pattern" and "flags" string fields
===DONE===
