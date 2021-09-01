--TEST--
MongoDB\BSON\DBPointer unserialization requires "id" string field to be valid (__serialize and __unserialize)
--SKIPIF--
<?php require __DIR__ . "/../utils/basic-skipif.inc"; ?>
<?php skip_if_php_version('<', '7.4.0'); ?>
--FILE--
<?php

require_once __DIR__ . '/../utils/basic.inc';


echo throws(function() {
	unserialize('O:22:"MongoDB\BSON\DBPointer":2:{s:3:"ref";s:11:"phongo.test";s:2:"id";s:24:"QQQQ78accd485d55b4050000";}');
}, 'MongoDB\Driver\Exception\InvalidArgumentException'), "\n";

echo throws(function() {
	unserialize('O:22:"MongoDB\BSON\DBPointer":2:{s:3:"ref";s:11:"phongo.test";s:2:"id";s:23:"52e78accd485d55b4050000";}');
}, 'MongoDB\Driver\Exception\InvalidArgumentException'), "\n";

?>
===DONE===
<?php exit(0); ?>
--EXPECT--
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Error parsing ObjectId string: QQQQ78accd485d55b4050000
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Error parsing ObjectId string: 52e78accd485d55b4050000
===DONE===
