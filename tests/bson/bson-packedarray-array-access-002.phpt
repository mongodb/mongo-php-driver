--TEST--
MongoDB\BSON\PackedArray array access (ArrayAccess methods)
--FILE--
<?php

require_once __DIR__ . '/../utils/basic.inc';

$array = MongoDB\BSON\PackedArray::fromPHP(["foo", new MongoDB\BSON\Int64(123)]);

var_dump($array->offsetExists(0));
var_dump($array->offsetExists(1));
var_dump($array->offsetExists(2));
var_dump($array->offsetExists('foo'));

var_dump($array->offsetGet(0));
var_dump($array->offsetGet(1));

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
bool(true)
bool(true)
bool(false)
bool(false)
string(3) "foo"
object(MongoDB\BSON\Int64)#%d (%d) {
  ["integer"]=>
  string(3) "123"
}
===DONE===
