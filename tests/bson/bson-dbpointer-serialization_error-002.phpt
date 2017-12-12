--TEST--
MongoDB\BSON\DBPointer unserialization requires "id" string field to be valid
--FILE--
<?php

require_once __DIR__ . '/../utils/tools.php';


echo throws(function() {
	unserialize('C:22:"MongoDB\BSON\DBPointer":76:{a:2:{s:3:"ref";s:11:"phongo.test";s:2:"id";s:24:"QQQQ78accd485d55b4050000";}}');
}, 'MongoDB\Driver\Exception\InvalidArgumentException'), "\n";

echo throws(function() {
	unserialize('C:22:"MongoDB\BSON\DBPointer":75:{a:2:{s:3:"ref";s:11:"phongo.test";s:2:"id";s:23:"52e78accd485d55b4050000";}}');
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
