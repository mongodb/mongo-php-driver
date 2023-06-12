--TEST--
MongoDB\BSON\Value::__set_state()
--SKIPIF--
<?php require __DIR__ . "/../utils/basic-skipif.inc"; ?>
<?php skip_if_php_version('<', '7.3.0'); ?>
--FILE--
<?php

require_once __DIR__ . '/../utils/basic.inc';

$tests = [
    'null' => null,
    'int32' => 2,
    'int64' => new MongoDB\BSON\Int64(2),
    'float' => 3.14,
    'string' => 'foo',
    'true' => true,
    'false' => false,
    'list' => [1, 2, 3],
    'packedArray' => MongoDB\BSON\PackedArray::fromPHP([1, 2, 3]),
    'struct' => ['foo' => 'bar'],
    'object' => (object) ['foo' => 'bar'],
    'document' => MongoDB\BSON\Document::fromPHP((object) ['foo' => 'bar']),
    'binary' => new MongoDB\BSON\Binary('foo'),
    'code' => new MongoDB\BSON\Javascript('foo'),
    'codeWithScope' => new MongoDB\BSON\Javascript('foo', ['foo' => 'bar']),
    'timestamp' => new MongoDB\BSON\Timestamp(1, 2),
    'decimal128' => new MongoDB\BSON\Decimal128('1.2'),
    'minKey' => new MongoDB\BSON\MinKey,
    'maxKey' => new MongoDB\BSON\MaxKey,
    'utcDateTime' => new MongoDB\BSON\UTCDateTime(1234567890),
    'regex' => new MongoDB\BSON\Regex('foo'),
    'dbPointer' => MongoDB\BSON\toPHP(MongoDB\BSON\fromJSON('{ "dbref": {"$dbPointer": {"$ref": "phongo.test", "$id" : { "$oid" : "5a2e78accd485d55b4050000" }  }} }'))->dbref,
    'symbol' => MongoDB\BSON\toPHP(MongoDB\BSON\fromJSON('{ "symbol": {"$symbol": "test"} }'))->symbol,
    'objectId' => new MongoDB\BSON\ObjectId('644a340b641850b1d0062d50'),
    'undefined' => MongoDB\BSON\toPHP(MongoDB\BSON\fromJSON('{ "undefined": {"$undefined": true} }'))->undefined,
];

foreach ($tests as $name => $value) {
    printf("Testing %s:\n", $name);

    $bsonValue = MongoDB\BSON\Value::fromPHP($value);
    $exported = var_export($bsonValue, true);
    echo $exported, "\n";

    var_dump(eval('return ' . $exported . ';'));
}

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
Testing null:
%r\\?%rMongoDB\BSON\Value::__set_state(array(
   'type' => 10,
   'value' => NULL,
))
object(MongoDB\BSON\Value)#%d (%d) {
  ["type"]=>
  string(4) "null"
  ["value"]=>
  NULL
}
Testing int32:
%r\\?%rMongoDB\BSON\Value::__set_state(array(
   'type' => 16,
   'value' => 2,
))
object(MongoDB\BSON\Value)#%d (%d) {
  ["type"]=>
  string(14) "32-bit integer"
  ["value"]=>
  int(2)
}
Testing int64:
%r\\?%rMongoDB\BSON\Value::__set_state(array(
   'type' => 18,
   'value' =>%w
  %r\\?%rMongoDB\BSON\Int64::__set_state(array(
     'integer' => '2',
  )),
))
object(MongoDB\BSON\Value)#%d (%d) {
  ["type"]=>
  string(14) "64-bit integer"
  ["value"]=>
  object(MongoDB\BSON\Int64)#%d (%d) {
    ["integer"]=>
    string(1) "2"
  }
}
Testing float:
%r\\?%rMongoDB\BSON\Value::__set_state(array(
   'type' => 1,
   'value' => 3.14,
))
object(MongoDB\BSON\Value)#%d (%d) {
  ["type"]=>
  string(6) "double"
  ["value"]=>
  float(3.14)
}
Testing string:
%r\\?%rMongoDB\BSON\Value::__set_state(array(
   'type' => 2,
   'value' => 'foo',
))
object(MongoDB\BSON\Value)#%d (%d) {
  ["type"]=>
  string(6) "string"
  ["value"]=>
  string(3) "foo"
}
Testing true:
%r\\?%rMongoDB\BSON\Value::__set_state(array(
   'type' => 8,
   'value' => true,
))
object(MongoDB\BSON\Value)#%d (%d) {
  ["type"]=>
  string(7) "boolean"
  ["value"]=>
  bool(true)
}
Testing false:
%r\\?%rMongoDB\BSON\Value::__set_state(array(
   'type' => 8,
   'value' => false,
))
object(MongoDB\BSON\Value)#%d (%d) {
  ["type"]=>
  string(7) "boolean"
  ["value"]=>
  bool(false)
}
Testing list:
%r\\?%rMongoDB\BSON\Value::__set_state(array(
   'type' => 4,
   'value' =>%w
  %r\\?%rMongoDB\BSON\PackedArray::__set_state(array(
     'data' => 'GgAAABAwAAEAAAAQMQACAAAAEDIAAwAAAAA=',
  )),
))
object(MongoDB\BSON\Value)#%d (%d) {
  ["type"]=>
  string(5) "array"
  ["value"]=>
  object(MongoDB\BSON\PackedArray)#%d (%d) {
    ["data"]=>
    string(36) "GgAAABAwAAEAAAAQMQACAAAAEDIAAwAAAAA="
    ["value"]=>
    array(3) {
      [0]=>
      int(1)
      [1]=>
      int(2)
      [2]=>
      int(3)
    }
  }
}
Testing packedArray:
%r\\?%rMongoDB\BSON\Value::__set_state(array(
   'type' => 4,
   'value' =>%w
  %r\\?%rMongoDB\BSON\PackedArray::__set_state(array(
     'data' => 'GgAAABAwAAEAAAAQMQACAAAAEDIAAwAAAAA=',
  )),
))
object(MongoDB\BSON\Value)#%d (%d) {
  ["type"]=>
  string(5) "array"
  ["value"]=>
  object(MongoDB\BSON\PackedArray)#%d (%d) {
    ["data"]=>
    string(36) "GgAAABAwAAEAAAAQMQACAAAAEDIAAwAAAAA="
    ["value"]=>
    array(3) {
      [0]=>
      int(1)
      [1]=>
      int(2)
      [2]=>
      int(3)
    }
  }
}
Testing struct:
%r\\?%rMongoDB\BSON\Value::__set_state(array(
   'type' => 3,
   'value' =>%w
  %r\\?%rMongoDB\BSON\Document::__set_state(array(
     'data' => 'EgAAAAJmb28ABAAAAGJhcgAA',
  )),
))
object(MongoDB\BSON\Value)#%d (%d) {
  ["type"]=>
  string(8) "document"
  ["value"]=>
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
Testing object:
%r\\?%rMongoDB\BSON\Value::__set_state(array(
   'type' => 3,
   'value' =>%w
  %r\\?%rMongoDB\BSON\Document::__set_state(array(
     'data' => 'EgAAAAJmb28ABAAAAGJhcgAA',
  )),
))
object(MongoDB\BSON\Value)#%d (%d) {
  ["type"]=>
  string(8) "document"
  ["value"]=>
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
Testing document:
%r\\?%rMongoDB\BSON\Value::__set_state(array(
   'type' => 3,
   'value' =>%w
  %r\\?%rMongoDB\BSON\Document::__set_state(array(
     'data' => 'EgAAAAJmb28ABAAAAGJhcgAA',
  )),
))
object(MongoDB\BSON\Value)#%d (%d) {
  ["type"]=>
  string(8) "document"
  ["value"]=>
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
Testing binary:
%r\\?%rMongoDB\BSON\Value::__set_state(array(
   'type' => 5,
   'value' =>%w
  %r\\?%rMongoDB\BSON\Binary::__set_state(array(
     'data' => 'foo',
     'type' => 0,
  )),
))
object(MongoDB\BSON\Value)#%d (%d) {
  ["type"]=>
  string(6) "Binary"
  ["value"]=>
  object(MongoDB\BSON\Binary)#%d (%d) {
    ["data"]=>
    string(3) "foo"
    ["type"]=>
    int(0)
  }
}
Testing code:
%r\\?%rMongoDB\BSON\Value::__set_state(array(
   'type' => 13,
   'value' =>%w
  %r\\?%rMongoDB\BSON\Javascript::__set_state(array(
     'code' => 'foo',
     'scope' => NULL,
  )),
))
object(MongoDB\BSON\Value)#%d (%d) {
  ["type"]=>
  string(10) "Javascript"
  ["value"]=>
  object(MongoDB\BSON\Javascript)#%d (%d) {
    ["code"]=>
    string(3) "foo"
    ["scope"]=>
    NULL
  }
}
Testing codeWithScope:
%r\\?%rMongoDB\BSON\Value::__set_state(array(
   'type' => 15,
   'value' =>%w
  %r\\?%rMongoDB\BSON\Javascript::__set_state(array(
     'code' => 'foo',
     'scope' =>%w
    %r\(object\) |stdClass::__set_state\(%rarray(
       'foo' => 'bar',
    %r\)?%r),
  )),
))
object(MongoDB\BSON\Value)#%d (%d) {
  ["type"]=>
  string(21) "Javascript with scope"
  ["value"]=>
  object(MongoDB\BSON\Javascript)#%d (%d) {
    ["code"]=>
    string(3) "foo"
    ["scope"]=>
    object(stdClass)#%d (%d) {
      ["foo"]=>
      string(3) "bar"
    }
  }
}
Testing timestamp:
%r\\?%rMongoDB\BSON\Value::__set_state(array(
   'type' => 17,
   'value' =>%w
  %r\\?%rMongoDB\BSON\Timestamp::__set_state(array(
     'increment' => '1',
     'timestamp' => '2',
  )),
))
object(MongoDB\BSON\Value)#%d (%d) {
  ["type"]=>
  string(9) "Timestamp"
  ["value"]=>
  object(MongoDB\BSON\Timestamp)#%d (%d) {
    ["increment"]=>
    string(1) "1"
    ["timestamp"]=>
    string(1) "2"
  }
}
Testing decimal128:
%r\\?%rMongoDB\BSON\Value::__set_state(array(
   'type' => 19,
   'value' =>%w
  %r\\?%rMongoDB\BSON\Decimal128::__set_state(array(
     'dec' => '1.2',
  )),
))
object(MongoDB\BSON\Value)#%d (%d) {
  ["type"]=>
  string(10) "Decimal128"
  ["value"]=>
  object(MongoDB\BSON\Decimal128)#%d (%d) {
    ["dec"]=>
    string(3) "1.2"
  }
}
Testing minKey:
%r\\?%rMongoDB\BSON\Value::__set_state(array(
   'type' => 255,
   'value' =>%w
  %r\\?%rMongoDB\BSON\MinKey::__set_state(array(
  )),
))
object(MongoDB\BSON\Value)#%d (%d) {
  ["type"]=>
  string(6) "MinKey"
  ["value"]=>
  object(MongoDB\BSON\MinKey)#%d (%d) {
  }
}
Testing maxKey:
%r\\?%rMongoDB\BSON\Value::__set_state(array(
   'type' => 127,
   'value' =>%w
  %r\\?%rMongoDB\BSON\MaxKey::__set_state(array(
  )),
))
object(MongoDB\BSON\Value)#%d (%d) {
  ["type"]=>
  string(6) "MaxKey"
  ["value"]=>
  object(MongoDB\BSON\MaxKey)#%d (%d) {
  }
}
Testing utcDateTime:
%r\\?%rMongoDB\BSON\Value::__set_state(array(
   'type' => 9,
   'value' =>%w
  %r\\?%rMongoDB\BSON\UTCDateTime::__set_state(array(
     'milliseconds' => '1234567890',
  )),
))
object(MongoDB\BSON\Value)#%d (%d) {
  ["type"]=>
  string(11) "UTCDateTime"
  ["value"]=>
  object(MongoDB\BSON\UTCDateTime)#%d (%d) {
    ["milliseconds"]=>
    string(10) "1234567890"
  }
}
Testing regex:
%r\\?%rMongoDB\BSON\Value::__set_state(array(
   'type' => 11,
   'value' =>%w
  %r\\?%rMongoDB\BSON\Regex::__set_state(array(
     'pattern' => 'foo',
     'flags' => '',
  )),
))
object(MongoDB\BSON\Value)#%d (%d) {
  ["type"]=>
  string(5) "Regex"
  ["value"]=>
  object(MongoDB\BSON\Regex)#%d (%d) {
    ["pattern"]=>
    string(3) "foo"
    ["flags"]=>
    string(0) ""
  }
}
Testing dbPointer:
%r\\?%rMongoDB\BSON\Value::__set_state(array(
   'type' => 12,
   'value' =>%w
  %r\\?%rMongoDB\BSON\DBPointer::__set_state(array(
     'ref' => 'phongo.test',
     'id' => '5a2e78accd485d55b4050000',
  )),
))
object(MongoDB\BSON\Value)#%d (%d) {
  ["type"]=>
  string(9) "DBPointer"
  ["value"]=>
  object(MongoDB\BSON\DBPointer)#%d (%d) {
    ["ref"]=>
    string(11) "phongo.test"
    ["id"]=>
    string(24) "5a2e78accd485d55b4050000"
  }
}
Testing symbol:
%r\\?%rMongoDB\BSON\Value::__set_state(array(
   'type' => 14,
   'value' =>%w
  %r\\?%rMongoDB\BSON\Symbol::__set_state(array(
     'symbol' => 'test',
  )),
))
object(MongoDB\BSON\Value)#%d (%d) {
  ["type"]=>
  string(6) "symbol"
  ["value"]=>
  object(MongoDB\BSON\Symbol)#%d (%d) {
    ["symbol"]=>
    string(4) "test"
  }
}
Testing objectId:
%r\\?%rMongoDB\BSON\Value::__set_state(array(
   'type' => 7,
   'value' =>%w
  %r\\?%rMongoDB\BSON\ObjectId::__set_state(array(
     'oid' => '644a340b641850b1d0062d50',
  )),
))
object(MongoDB\BSON\Value)#%d (%d) {
  ["type"]=>
  string(8) "ObjectId"
  ["value"]=>
  object(MongoDB\BSON\ObjectId)#%d (%d) {
    ["oid"]=>
    string(24) "644a340b641850b1d0062d50"
  }
}
Testing undefined:
%r\\?%rMongoDB\BSON\Value::__set_state(array(
   'type' => 6,
   'value' =>%w
  %r\\?%rMongoDB\BSON\Undefined::__set_state(array(
  )),
))
object(MongoDB\BSON\Value)#%d (%d) {
  ["type"]=>
  string(9) "undefined"
  ["value"]=>
  object(MongoDB\BSON\Undefined)#%d (%d) {
  }
}
===DONE===
