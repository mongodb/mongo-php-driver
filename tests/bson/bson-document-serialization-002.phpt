--TEST--
MongoDB\BSON\Document serialization (__serialize and __unserialize)
--SKIPIF--
<?php require __DIR__ . "/../utils/basic-skipif.inc"; ?>
<?php skip_if_php_version('<', '7.4.0'); ?>
--FILE--
<?php

$document = MongoDB\BSON\Document::fromPHP(['_id' => 1, 'array' => [0, 1, 'foo'], 'document' => ['foo' => 'bar']]);
var_dump($document);
var_dump($s = serialize($document));
var_dump(unserialize($s));

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
object(MongoDB\BSON\Document)#%d (%d) {
  ["data"]=>
  string(108) "TwAAABBfaWQAAQAAAARhcnJheQAeAAAAEDAAAAAAABAxAAEAAAACMgAEAAAAZm9vAAADZG9jdW1lbnQAEgAAAAJmb28ABAAAAGJhcgAAAA=="
  ["value"]=>
  object(stdClass)#%d (%d) {
    ["_id"]=>
    int(1)
    ["array"]=>
    object(MongoDB\BSON\PackedArray)#%d (%d) {
      ["data"]=>
      string(40) "HgAAABAwAAAAAAAQMQABAAAAAjIABAAAAGZvbwAA"
      ["value"]=>
      array(3) {
        [0]=>
        int(0)
        [1]=>
        int(1)
        [2]=>
        string(3) "foo"
      }
    }
    ["document"]=>
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
string(161) "O:21:"MongoDB\BSON\Document":1:{s:4:"data";s:108:"TwAAABBfaWQAAQAAAARhcnJheQAeAAAAEDAAAAAAABAxAAEAAAACMgAEAAAAZm9vAAADZG9jdW1lbnQAEgAAAAJmb28ABAAAAGJhcgAAAA==";}"
object(MongoDB\BSON\Document)#%d (%d) {
  ["data"]=>
  string(108) "TwAAABBfaWQAAQAAAARhcnJheQAeAAAAEDAAAAAAABAxAAEAAAACMgAEAAAAZm9vAAADZG9jdW1lbnQAEgAAAAJmb28ABAAAAGJhcgAAAA=="
  ["value"]=>
  object(stdClass)#%d (%d) {
    ["_id"]=>
    int(1)
    ["array"]=>
    object(MongoDB\BSON\PackedArray)#%d (%d) {
      ["data"]=>
      string(40) "HgAAABAwAAAAAAAQMQABAAAAAjIABAAAAGZvbwAA"
      ["value"]=>
      array(3) {
        [0]=>
        int(0)
        [1]=>
        int(1)
        [2]=>
        string(3) "foo"
      }
    }
    ["document"]=>
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
