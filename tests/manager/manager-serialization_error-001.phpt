--TEST--
MongoDB\Driver\Manager does not support serialization (PHP >= 8.1)
--SKIPIF--
<?php require __DIR__ . "/../utils/basic-skipif.inc"; ?>
<?php skip_if_php_version('<', '8.1'); ?>
--FILE--
<?php
 
require_once __DIR__ . '/../utils/basic.inc';
 
echo throws(function() {
    serialize(create_test_manager());
}, Exception::class), "\n";
 
echo throws(function() {
    unserialize('C:22:"MongoDB\Driver\Manager":0:{}');
}, Exception::class), "\n";
 
echo throws(function() {
    unserialize('O:22:"MongoDB\Driver\Manager":0:{}');
}, Exception::class), "\n";
 
?>
===DONE===
<?php exit(0); ?>
--EXPECT--
OK: Got Exception
Serialization of 'MongoDB\Driver\Manager' is not allowed
OK: Got Exception
Unserialization of 'MongoDB\Driver\Manager' is not allowed
OK: Got Exception
Unserialization of 'MongoDB\Driver\Manager' is not allowed
===DONE===
