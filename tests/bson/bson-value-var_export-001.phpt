--TEST--
MongoDB\BSON\Value: var_export tests
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
    echo var_export($bsonValue, true), "\n";
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
Testing int32:
%r\\?%rMongoDB\BSON\Value::__set_state(array(
   'type' => 16,
   'value' => 2,
))
Testing int64:
%r\\?%rMongoDB\BSON\Value::__set_state(array(
   'type' => 18,
   'value' =>%w
  %r\\?%rMongoDB\BSON\Int64::__set_state(array(
     'integer' => '2',
  )),
))
Testing float:
%r\\?%rMongoDB\BSON\Value::__set_state(array(
   'type' => 1,
   'value' => 3.14,
))
Testing string:
%r\\?%rMongoDB\BSON\Value::__set_state(array(
   'type' => 2,
   'value' => 'foo',
))
Testing true:
%r\\?%rMongoDB\BSON\Value::__set_state(array(
   'type' => 8,
   'value' => true,
))
Testing false:
%r\\?%rMongoDB\BSON\Value::__set_state(array(
   'type' => 8,
   'value' => false,
))
Testing list:
%r\\?%rMongoDB\BSON\Value::__set_state(array(
   'type' => 4,
   'value' =>%w
  %r\\?%rMongoDB\BSON\PackedArray::__set_state(array(
     'data' => 'GgAAABAwAAEAAAAQMQACAAAAEDIAAwAAAAA=',
  )),
))
Testing packedArray:
%r\\?%rMongoDB\BSON\Value::__set_state(array(
   'type' => 4,
   'value' =>%w
  %r\\?%rMongoDB\BSON\PackedArray::__set_state(array(
     'data' => 'GgAAABAwAAEAAAAQMQACAAAAEDIAAwAAAAA=',
  )),
))
Testing struct:
%r\\?%rMongoDB\BSON\Value::__set_state(array(
   'type' => 3,
   'value' =>%w
  %r\\?%rMongoDB\BSON\Document::__set_state(array(
     'data' => 'EgAAAAJmb28ABAAAAGJhcgAA',
  )),
))
Testing object:
%r\\?%rMongoDB\BSON\Value::__set_state(array(
   'type' => 3,
   'value' =>%w
  %r\\?%rMongoDB\BSON\Document::__set_state(array(
     'data' => 'EgAAAAJmb28ABAAAAGJhcgAA',
  )),
))
Testing document:
%r\\?%rMongoDB\BSON\Value::__set_state(array(
   'type' => 3,
   'value' =>%w
  %r\\?%rMongoDB\BSON\Document::__set_state(array(
     'data' => 'EgAAAAJmb28ABAAAAGJhcgAA',
  )),
))
Testing binary:
%r\\?%rMongoDB\BSON\Value::__set_state(array(
   'type' => 5,
   'value' =>%w
  %r\\?%rMongoDB\BSON\Binary::__set_state(array(
     'data' => 'foo',
     'type' => 0,
  )),
))
Testing code:
%r\\?%rMongoDB\BSON\Value::__set_state(array(
   'type' => 13,
   'value' =>%w
  %r\\?%rMongoDB\BSON\Javascript::__set_state(array(
     'code' => 'foo',
     'scope' => NULL,
  )),
))
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
Testing timestamp:
%r\\?%rMongoDB\BSON\Value::__set_state(array(
   'type' => 17,
   'value' =>%w
  %r\\?%rMongoDB\BSON\Timestamp::__set_state(array(
     'increment' => '1',
     'timestamp' => '2',
  )),
))
Testing decimal128:
%r\\?%rMongoDB\BSON\Value::__set_state(array(
   'type' => 19,
   'value' =>%w
  %r\\?%rMongoDB\BSON\Decimal128::__set_state(array(
     'dec' => '1.2',
  )),
))
Testing minKey:
%r\\?%rMongoDB\BSON\Value::__set_state(array(
   'type' => 255,
   'value' =>%w
  %r\\?%rMongoDB\BSON\MinKey::__set_state(array(
  )),
))
Testing maxKey:
%r\\?%rMongoDB\BSON\Value::__set_state(array(
   'type' => 127,
   'value' =>%w
  %r\\?%rMongoDB\BSON\MaxKey::__set_state(array(
  )),
))
Testing utcDateTime:
%r\\?%rMongoDB\BSON\Value::__set_state(array(
   'type' => 9,
   'value' =>%w
  %r\\?%rMongoDB\BSON\UTCDateTime::__set_state(array(
     'milliseconds' => '1234567890',
  )),
))
Testing regex:
%r\\?%rMongoDB\BSON\Value::__set_state(array(
   'type' => 11,
   'value' =>%w
  %r\\?%rMongoDB\BSON\Regex::__set_state(array(
     'pattern' => 'foo',
     'flags' => '',
  )),
))
Testing dbPointer:
%r\\?%rMongoDB\BSON\Value::__set_state(array(
   'type' => 12,
   'value' =>%w
  %r\\?%rMongoDB\BSON\DBPointer::__set_state(array(
     'ref' => 'phongo.test',
     'id' => '5a2e78accd485d55b4050000',
  )),
))
Testing symbol:
%r\\?%rMongoDB\BSON\Value::__set_state(array(
   'type' => 14,
   'value' =>%w
  %r\\?%rMongoDB\BSON\Symbol::__set_state(array(
     'symbol' => 'test',
  )),
))
Testing objectId:
%r\\?%rMongoDB\BSON\Value::__set_state(array(
   'type' => 7,
   'value' =>%w
  %r\\?%rMongoDB\BSON\ObjectId::__set_state(array(
     'oid' => '644a340b641850b1d0062d50',
  )),
))
Testing undefined:
%r\\?%rMongoDB\BSON\Value::__set_state(array(
   'type' => 6,
   'value' =>%w
  %r\\?%rMongoDB\BSON\Undefined::__set_state(array(
  )),
))
===DONE===
