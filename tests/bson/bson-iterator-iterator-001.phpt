--TEST--
MongoDB\BSON\Iterator iterating objects and arrays
--FILE--
<?php

require_once __DIR__ . '/../utils/basic.inc';

$data = [
    'objectId' => new MongoDB\BSON\ObjectId('56315a7c6118fd1b920270b1'),
    'string' => 'foo',
    'int32' => 123,
    'int64' => new MongoDB\BSON\Int64(123),
    'float' => 3.1415926,
    'document' => MongoDB\BSON\Document::fromJSON('{ "foo": "bar" }'),
    'object' => (object) ['foo' => 'bar'],
    'bson_array' => MongoDB\BSON\PackedArray::fromPHP([1, 2, 'foo']),
    'array' => [0, 1, 'bar'],
];

$iterator = MongoDB\BSON\Document::fromPHP($data)->getIterator();
var_dump(iterator_to_array($iterator));

$iterator = MongoDB\BSON\PackedArray::fromPHP(array_values($data))->getIterator();
var_dump(iterator_to_array($iterator));

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
array(9) {
  ["objectId"]=>
  object(MongoDB\BSON\ObjectId)#%d (%d) {
    ["oid"]=>
    string(24) "56315a7c6118fd1b920270b1"
  }
  ["string"]=>
  string(3) "foo"
  ["int32"]=>
  int(123)
  ["int64"]=>
  object(MongoDB\BSON\Int64)#%d (%d) {
    ["integer"]=>
    string(3) "123"
  }
  ["float"]=>
  float(3.1415926)
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
  ["object"]=>
  object(MongoDB\BSON\Document)#%d (%d) {
    ["data"]=>
    string(24) "EgAAAAJmb28ABAAAAGJhcgAA"
    ["value"]=>
    object(stdClass)#%d (%d) {
      ["foo"]=>
      string(3) "bar"
    }
  }
  ["bson_array"]=>
  object(MongoDB\BSON\PackedArray)#%d (%d) {
    ["data"]=>
    string(40) "HgAAABAwAAEAAAAQMQACAAAAAjIABAAAAGZvbwAA"
    ["value"]=>
    array(3) {
      [0]=>
      int(1)
      [1]=>
      int(2)
      [2]=>
      string(3) "foo"
    }
  }
  ["array"]=>
  object(MongoDB\BSON\PackedArray)#%d (%d) {
    ["data"]=>
    string(40) "HgAAABAwAAAAAAAQMQABAAAAAjIABAAAAGJhcgAA"
    ["value"]=>
    array(3) {
      [0]=>
      int(0)
      [1]=>
      int(1)
      [2]=>
      string(3) "bar"
    }
  }
}
array(9) {
  [0]=>
  object(MongoDB\BSON\ObjectId)#%d (%d) {
    ["oid"]=>
    string(24) "56315a7c6118fd1b920270b1"
  }
  [1]=>
  string(3) "foo"
  [2]=>
  int(123)
  [3]=>
  object(MongoDB\BSON\Int64)#%d (%d) {
    ["integer"]=>
    string(3) "123"
  }
  [4]=>
  float(3.1415926)
  [5]=>
  object(MongoDB\BSON\Document)#%d (%d) {
    ["data"]=>
    string(24) "EgAAAAJmb28ABAAAAGJhcgAA"
    ["value"]=>
    object(stdClass)#%d (%d) {
      ["foo"]=>
      string(3) "bar"
    }
  }
  [6]=>
  object(MongoDB\BSON\Document)#%d (%d) {
    ["data"]=>
    string(24) "EgAAAAJmb28ABAAAAGJhcgAA"
    ["value"]=>
    object(stdClass)#%d (%d) {
      ["foo"]=>
      string(3) "bar"
    }
  }
  [7]=>
  object(MongoDB\BSON\PackedArray)#%d (%d) {
    ["data"]=>
    string(40) "HgAAABAwAAEAAAAQMQACAAAAAjIABAAAAGZvbwAA"
    ["value"]=>
    array(3) {
      [0]=>
      int(1)
      [1]=>
      int(2)
      [2]=>
      string(3) "foo"
    }
  }
  [8]=>
  object(MongoDB\BSON\PackedArray)#%d (%d) {
    ["data"]=>
    string(40) "HgAAABAwAAAAAAAQMQABAAAAAjIABAAAAGJhcgAA"
    ["value"]=>
    array(3) {
      [0]=>
      int(0)
      [1]=>
      int(1)
      [2]=>
      string(3) "bar"
    }
  }
}
===DONE===
