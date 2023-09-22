--TEST--
MongoDB\BSON\PackedArray array access (dimension object accessors)
--FILE--
<?php

require_once __DIR__ . '/../utils/basic.inc';

$array = MongoDB\BSON\PackedArray::fromPHP(["foo", new MongoDB\BSON\Int64(123)]);

var_dump(isset($array[0]));
var_dump(isset($array[1]));
var_dump(isset($array[2]));
var_dump(isset($array['foo']));

var_dump($array[0]);
var_dump($array[1]);

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
