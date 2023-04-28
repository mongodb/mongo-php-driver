--TEST--
MongoDB\BSON\Value: var_export 64-bit values (64-bit platforms)
--SKIPIF--
<?php if (8 !== PHP_INT_SIZE) { die('skip Only for 64-bit platform'); } ?>
--FILE--
<?php

require_once __DIR__ . '/../utils/basic.inc';

$bsonValue = MongoDB\BSON\Value::fromPHP(2147483648);
echo var_export($bsonValue, true), "\n";

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
%r\\?%rMongoDB\BSON\Value::__set_state(array(
   'type' => 18,
   'value' => '2147483648',
))
===DONE===
