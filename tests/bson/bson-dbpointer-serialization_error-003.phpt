--TEST--
MongoDB\BSON\DBPointer unserialization requires "ref" and "id" string fields (__serialize and __unserialize)
--FILE--
<?php

require_once __DIR__ . '/../utils/basic.inc';


echo throws(function() {
	unserialize('O:22:"MongoDB\BSON\DBPointer":1:{s:3:"ref";s:11:"phongo.test";}');
}, 'MongoDB\Driver\Exception\InvalidArgumentException'), "\n";

echo throws(function() {
	unserialize('O:22:"MongoDB\BSON\DBPointer":1:{s:2:"id";s:11:"phongo.test";}');
}, 'MongoDB\Driver\Exception\InvalidArgumentException'), "\n";

?>
===DONE===
<?php exit(0); ?>
--EXPECT--
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
MongoDB\BSON\DBPointer initialization requires "ref" and "id" string fields
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
MongoDB\BSON\DBPointer initialization requires "ref" and "id" string fields
===DONE===
