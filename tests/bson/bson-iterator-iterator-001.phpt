--TEST--
MongoDB\BSON\Iterator iterating objects and arrays
--FILE--
<?php

require_once __DIR__ . '/../utils/basic.inc';

$data = [
    'objectId' => new MongoDB\BSON\ObjectId('56315a7c6118fd1b920270b1'),
    'string' => 'foo',
    'int' => 123,
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
array(8) {
  ["objectId"]=>
  object(MongoDB\BSON\Value)#%d (%d) {
    ["type"]=>
    string(8) "ObjectId"
    ["value"]=>
    object(MongoDB\BSON\ObjectId)#%d (%d) {
      ["oid"]=>
      string(24) "56315a7c6118fd1b920270b1"
    }
  }
  ["string"]=>
  object(MongoDB\BSON\Value)#%d (%d) {
    ["type"]=>
    string(6) "string"
    ["value"]=>
    string(3) "foo"
  }
  ["int"]=>
  object(MongoDB\BSON\Value)#%d (%d) {
    ["type"]=>
    string(14) "32-bit integer"
    ["value"]=>
    int(123)
  }
  ["float"]=>
  object(MongoDB\BSON\Value)#%d (%d) {
    ["type"]=>
    string(6) "double"
    ["value"]=>
    float(3.1415926)
  }
  ["document"]=>
  object(MongoDB\BSON\Value)#%d (%d) {
    ["type"]=>
    string(8) "document"
    ["value"]=>
    object(MongoDB\BSON\Document)#%d (%d) {
      ["data"]=>
      string(24) "EgAAAAJmb28ABAAAAGJhcgAA"
    }
  }
  ["object"]=>
  object(MongoDB\BSON\Value)#%d (%d) {
    ["type"]=>
    string(8) "document"
    ["value"]=>
    object(MongoDB\BSON\Document)#%d (%d) {
      ["data"]=>
      string(24) "EgAAAAJmb28ABAAAAGJhcgAA"
    }
  }
  ["bson_array"]=>
  object(MongoDB\BSON\Value)#%d (%d) {
    ["type"]=>
    string(5) "array"
    ["value"]=>
    object(MongoDB\BSON\PackedArray)#%d (%d) {
      ["data"]=>
      string(40) "HgAAABAwAAEAAAAQMQACAAAAAjIABAAAAGZvbwAA"
    }
  }
  ["array"]=>
  object(MongoDB\BSON\Value)#%d (%d) {
    ["type"]=>
    string(5) "array"
    ["value"]=>
    object(MongoDB\BSON\PackedArray)#%d (%d) {
      ["data"]=>
      string(40) "HgAAABAwAAAAAAAQMQABAAAAAjIABAAAAGJhcgAA"
    }
  }
}
array(8) {
  [0]=>
  object(MongoDB\BSON\Value)#%d (%d) {
    ["type"]=>
    string(8) "ObjectId"
    ["value"]=>
    object(MongoDB\BSON\ObjectId)#%d (%d) {
      ["oid"]=>
      string(24) "56315a7c6118fd1b920270b1"
    }
  }
  [1]=>
  object(MongoDB\BSON\Value)#%d (%d) {
    ["type"]=>
    string(6) "string"
    ["value"]=>
    string(3) "foo"
  }
  [2]=>
  object(MongoDB\BSON\Value)#%d (%d) {
    ["type"]=>
    string(14) "32-bit integer"
    ["value"]=>
    int(123)
  }
  [3]=>
  object(MongoDB\BSON\Value)#%d (%d) {
    ["type"]=>
    string(6) "double"
    ["value"]=>
    float(3.1415926)
  }
  [4]=>
  object(MongoDB\BSON\Value)#%d (%d) {
    ["type"]=>
    string(8) "document"
    ["value"]=>
    object(MongoDB\BSON\Document)#%d (%d) {
      ["data"]=>
      string(24) "EgAAAAJmb28ABAAAAGJhcgAA"
    }
  }
  [5]=>
  object(MongoDB\BSON\Value)#%d (%d) {
    ["type"]=>
    string(8) "document"
    ["value"]=>
    object(MongoDB\BSON\Document)#%d (%d) {
      ["data"]=>
      string(24) "EgAAAAJmb28ABAAAAGJhcgAA"
    }
  }
  [6]=>
  object(MongoDB\BSON\Value)#%d (%d) {
    ["type"]=>
    string(5) "array"
    ["value"]=>
    object(MongoDB\BSON\PackedArray)#%d (%d) {
      ["data"]=>
      string(40) "HgAAABAwAAEAAAAQMQACAAAAAjIABAAAAGZvbwAA"
    }
  }
  [7]=>
  object(MongoDB\BSON\Value)#%d (%d) {
    ["type"]=>
    string(5) "array"
    ["value"]=>
    object(MongoDB\BSON\PackedArray)#%d (%d) {
      ["data"]=>
      string(40) "HgAAABAwAAAAAAAQMQABAAAAAjIABAAAAGJhcgAA"
    }
  }
}
===DONE===
