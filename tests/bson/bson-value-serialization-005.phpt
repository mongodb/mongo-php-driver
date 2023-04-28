--TEST--
MongoDB\BSON\Value serialization (__serialize and __unserialize): 64-bit integer (64-bit platforms)
--SKIPIF--
<?php require __DIR__ . "/../utils/basic-skipif.inc"; ?>
<?php skip_if_php_version('<', '7.4.0'); ?>
<?php if (8 !== PHP_INT_SIZE) { die('skip Only for 64-bit platform'); } ?>
--FILE--
<?php

require_once __DIR__ . '/../utils/basic.inc';

var_dump($bsonValue = MongoDB\BSON\Value::fromPHP(2147483648));
var_dump($s = serialize($bsonValue));
var_dump(unserialize($s));

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
object(MongoDB\BSON\Value)#%d (%d) {
  ["type"]=>
  string(14) "64-bit integer"
  ["value"]=>
  int(2147483648)
}
string(76) "O:18:"MongoDB\BSON\Value":2:{s:4:"type";i:18;s:5:"value";s:10:"2147483648";}"
object(MongoDB\BSON\Value)#%d (%d) {
  ["type"]=>
  string(14) "64-bit integer"
  ["value"]=>
  int(2147483648)
}
===DONE===
