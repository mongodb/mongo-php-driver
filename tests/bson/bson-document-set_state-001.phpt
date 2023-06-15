--TEST--
MongoDB\BSON\Document::__set_state()
--FILE--
<?php

$document = MongoDB\BSON\Document::fromPHP([
    '_id' => 1,
    'array' => [0, 1, 'foo'],
    'document' => ['foo' => 'bar']
]);

$s = var_export($document, true);
echo $s, "\n";

var_dump($o = eval('return ' . $s . ';'));
var_dump($o == $document);

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
%r\\?%rMongoDB\BSON\Document::__set_state(array(
%w'data' => 'TwAAABBfaWQAAQAAAARhcnJheQAeAAAAEDAAAAAAABAxAAEAAAACMgAEAAAAZm9vAAADZG9jdW1lbnQAEgAAAAJmb28ABAAAAGJhcgAAAA==',
))
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
bool(true)
===DONE===
