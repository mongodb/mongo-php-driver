--TEST--
MongoDB\BSON\PackedArray serialization (__serialize and __unserialize)
--SKIPIF--
<?php require __DIR__ . "/../utils/basic-skipif.inc"; ?>
<?php skip_if_php_version('<', '7.4.0'); ?>
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
  ["value"]=>
  array(4) {
    [0]=>
    int(0)
    [1]=>
    int(1)
    [2]=>
    string(3) "foo"
    [3]=>
    object(MongoDB\BSON\Document)#%d (%d) {
      ["data"]=>
      string(24) "EgAAAAJmb28ABAAAAGJhcgAA"
      ["value"]=>
      object(stdClass)#%d (%d) {
        ["foo"]=>
        string(3) "bar"
      }
    }
  }
}
string(123) "O:24:"MongoDB\BSON\PackedArray":1:{s:4:"data";s:68:"MwAAABAwAAAAAAAQMQABAAAAAjIABAAAAGZvbwADMwASAAAAAmZvbwAEAAAAYmFyAAAA";}"
object(MongoDB\BSON\PackedArray)#%d (%d) {
  ["data"]=>
  string(68) "MwAAABAwAAAAAAAQMQABAAAAAjIABAAAAGZvbwADMwASAAAAAmZvbwAEAAAAYmFyAAAA"
  ["value"]=>
  array(4) {
    [0]=>
    int(0)
    [1]=>
    int(1)
    [2]=>
    string(3) "foo"
    [3]=>
    object(MongoDB\BSON\Document)#%d (%d) {
      ["data"]=>
      string(24) "EgAAAAJmb28ABAAAAGJhcgAA"
      ["value"]=>
      object(stdClass)#%d (%d) {
        ["foo"]=>
        string(3) "bar"
      }
    }
  }
}
===DONE===
