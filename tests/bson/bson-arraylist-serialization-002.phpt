--TEST--
MongoDB\BSON\ArrayList serialization (__serialize and __unserialize)
--SKIPIF--
<?php require __DIR__ . "/../utils/basic-skipif.inc"; ?>
<?php skip_if_php_version('<', '7.4.0'); ?>
--FILE--
<?php

$array = MongoDB\BSON\ArrayList::fromPHP([0, 1, 'foo', (object) ['foo' => 'bar']]);
var_dump($array);
var_dump($s = serialize($array));
var_dump(unserialize($s));

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
object(MongoDB\BSON\ArrayList)#%d (%d) {
  ["data"]=>
  string(68) "MwAAABAwAAAAAAAQMQABAAAAAjIABAAAAGZvbwADMwASAAAAAmZvbwAEAAAAYmFyAAAA"
  ["length"]=>
  int(51)
}
string(139) "O:22:"MongoDB\BSON\ArrayList":2:{s:4:"data";s:68:"MwAAABAwAAAAAAAQMQABAAAAAjIABAAAAGZvbwADMwASAAAAAmZvbwAEAAAAYmFyAAAA";s:6:"length";i:51;}"
object(MongoDB\BSON\ArrayList)#%d (%d) {
  ["data"]=>
  string(68) "MwAAABAwAAAAAAAQMQABAAAAAjIABAAAAGZvbwADMwASAAAAAmZvbwAEAAAAYmFyAAAA"
  ["length"]=>
  int(51)
}
===DONE===
