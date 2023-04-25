--TEST--
MongoDB\BSON\Value::fromPHP(): 64-bit integer (64-bit platforms)
--SKIPIF--
<?php if (8 !== PHP_INT_SIZE) { die('skip Only for 64-bit platform'); } ?>
--FILE--
<?php

require_once __DIR__ . '/../utils/basic.inc';

$bsonValue = MongoDB\BSON\Value::fromPHP(2147483648);

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
int(2147483648)
int(2147483648)
bool(true)
bool(true)
===DONE===
