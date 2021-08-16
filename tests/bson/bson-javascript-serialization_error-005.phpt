--TEST--
MongoDB\BSON\Javascript unserialization expects optional scope to be array or object (__serialize and __unserialize)
--SKIPIF--
<?php require __DIR__ . "/../utils/basic-skipif.inc"; ?>
<?php skip_if_php_version('<', '7.4.0'); ?>
--FILE--
<?php

require_once __DIR__ . '/../utils/basic.inc';

echo throws(function() {
    unserialize('O:23:"MongoDB\BSON\Javascript":2:{s:4:"code";s:17:"function foo() {}";s:5:"scope";s:7:"INVALID";}');
}, 'MongoDB\Driver\Exception\InvalidArgumentException'), "\n";

?>
===DONE===
<?php exit(0); ?>
--EXPECT--
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Expected scope to be array or object, string given
===DONE===
