--TEST--
MongoDB\BSON\Value::fromPHP(): 64-bit integer (32-bit platforms)
--SKIPIF--
<?php if (4 !== PHP_INT_SIZE) { die('skip Only for 32-bit platform'); } ?>
--FILE--
<?php

require_once __DIR__ . '/../utils/basic.inc';

$int64 = unserialize('C:18:"MongoDB\BSON\Int64":38:{a:1:{s:7:"integer";s:10:"2147483648";}}');
$bsonValue = MongoDB\BSON\Value::fromPHP($int64);

var_dump($bsonValue);
var_dump($bsonValue->getValue());
var_dump($bsonValue->getInt64());
var_dump($bsonValue->getType() === MongoDB\BSON\Value::TYPE_INT64);
var_dump($bsonValue->isInt64());

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
object(MongoDB\BSON\Value)#%d (%d) {
}
object(MongoDB\BSON\Int64)#%d (%d) {
  ["integer"]=>
  string(10) "2147483648"
}
object(MongoDB\BSON\Int64)#%d (%d) {
  ["integer"]=>
  string(10) "2147483648"
}
bool(true)
bool(true)
===DONE===
