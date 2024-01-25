--TEST--
MongoDB\Driver\Manager does not support serialization (PHP < 8.1)
--SKIPIF--
<?php require __DIR__ . "/../utils/basic-skipif.inc"; ?>
<?php skip_if_php_version('>=', '8.1'); ?>
--FILE--
<?php
 
require_once __DIR__ . '/../utils/basic.inc';
 
echo throws(function() {
    serialize(create_test_manager());
}, Exception::class), "\n";
 
echo throws(function() {
    unserialize('C:22:"MongoDB\Driver\Manager":0:{}');
}, Exception::class), "\n";

/* Note: PHONGO_CE_DISABLE_SERIALIZATION assigns zend_class_entry.serialize to
 * zend_class_serialize_deny on PHP < 8.1, which triggers the following:
 * https://github.com/php/php-src/blob/php-8.0.30/ext/standard/var_unserializer.re#L1181
 */
echo raises(function() {
    unserialize('O:22:"MongoDB\Driver\Manager":0:{}');
}, E_WARNING), "\n";
 
?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
OK: Got Exception
Serialization of 'MongoDB\Driver\Manager' is not allowed
OK: Got Exception
Unserialization of 'MongoDB\Driver\Manager' is not allowed
OK: Got E_WARNING
Erroneous data format for unserializing 'MongoDB\Driver\Manager'
===DONE===
