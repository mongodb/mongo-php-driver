--TEST--
MongoDB\BSON\Value serialization (__serialize and __unserialize): 64-bit integer (32-bit platforms)
--SKIPIF--
<?php require __DIR__ . "/../utils/basic-skipif.inc"; ?>
<?php skip_if_php_version('<', '7.4.0'); ?>
<?php if (4 !== PHP_INT_SIZE) { die('skip Only for 32-bit platform'); } ?>
--FILE--
<?php

require_once __DIR__ . '/../utils/basic.inc';

$int64 = unserialize('C:18:"MongoDB\BSON\Int64":38:{a:1:{s:7:"integer";s:10:"2147483648";}}');
var_dump($bsonValue = MongoDB\BSON\Value::fromPHP($int64));
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
  object(MongoDB\BSON\Int64)#%d (%d) {
    ["integer"]=>
    string(10) "2147483648"
  }
}
string(76) "O:18:"MongoDB\BSON\Value":2:{s:4:"type";i:18;s:5:"value";s:10:"2147483648";}"
object(MongoDB\BSON\Value)#%d (%d) {
  ["type"]=>
  string(14) "64-bit integer"
  ["value"]=>
  object(MongoDB\BSON\Int64)#%d (%d) {
    ["integer"]=>
    string(10) "2147483648"
  }
}
===DONE===
