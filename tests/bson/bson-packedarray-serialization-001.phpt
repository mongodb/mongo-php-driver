--TEST--
MongoDB\BSON\PackedArray serialization (Serializable interface)
--SKIPIF--
<?php require __DIR__ . "/../utils/basic-skipif.inc"; ?>
<?php skip_if_php_version('>=', '7.4.0'); ?>
--FILE--
<?php

$array = MongoDB\BSON\PackedArray::fromPHP([0, 1, 'foo', (object) ['foo' => 'bar']]);
var_dump($array);
var_dump($s = serialize($array));
var_dump(unserialize($s));

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
object(MongoDB\BSON\PackedArray)#%d (%d) {
  ["data"]=>
  string(68) "MwAAABAwAAAAAAAQMQABAAAAAjIABAAAAGZvbwADMwASAAAAAmZvbwAEAAAAYmFyAAAA"
}
string(130) "C:24:"MongoDB\BSON\PackedArray":93:{a:1:{s:4:"data";s:68:"MwAAABAwAAAAAAAQMQABAAAAAjIABAAAAGZvbwADMwASAAAAAmZvbwAEAAAAYmFyAAAA";}}"
object(MongoDB\BSON\PackedArray)#%d (%d) {
  ["data"]=>
  string(68) "MwAAABAwAAAAAAAQMQABAAAAAjIABAAAAGZvbwADMwASAAAAAmZvbwAEAAAAYmFyAAAA"
}
===DONE===
