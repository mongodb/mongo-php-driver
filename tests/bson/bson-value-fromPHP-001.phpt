--TEST--
MongoDB\BSON\Value::fromPHP()
--FILE--
<?php

require_once __DIR__ . '/../utils/basic.inc';

$tests = [
    'null' => [
        null,
        MongoDB\BSON\Value::TYPE_NULL,
        'isNull',
        'getNull',
    ],
    'int32' => [
        2,
        MongoDB\BSON\Value::TYPE_INT32,
        'isInt32',
        'getInt32',
    ],
    'int64' => [
        unserialize('C:18:"MongoDB\BSON\Int64":28:{a:1:{s:7:"integer";s:1:"2";}}'),
        MongoDB\BSON\Value::TYPE_INT64,
        'isInt64',
        'getInt64',
    ],
    'float' => [
        3.14,
        MongoDB\BSON\Value::TYPE_DOUBLE,
        'isDouble',
        'getDouble',
    ],
    'string' => [
        'foo',
        MongoDB\BSON\Value::TYPE_UTF8,
        'isUtf8',
        'getUtf8',
    ],
    'true' => [
        true,
        MongoDB\BSON\Value::TYPE_BOOL,
        'isBool',
        'getBool',
    ],
    'false' => [
        false,
        MongoDB\BSON\Value::TYPE_BOOL,
        'isBool',
        'getBool',
    ],
    'list' => [
        [1, 2, 3],
        MongoDB\BSON\Value::TYPE_ARRAY,
        'isArray',
        'getArray',
    ],
    'packedArray' => [
        MongoDB\BSON\PackedArray::fromPHP([1, 2, 3]),
        MongoDB\BSON\Value::TYPE_ARRAY,
        'isArray',
        'getArray',
    ],
    'struct' => [
        ['foo' => 'bar'],
        MongoDB\BSON\Value::TYPE_DOCUMENT,
        'isDocument',
        'getDocument',
    ],
    'object' => [
        (object) ['foo' => 'bar'],
        MongoDB\BSON\Value::TYPE_DOCUMENT,
        'isDocument',
        'getDocument',
    ],
    'document' => [
        MongoDB\BSON\Document::fromPHP((object) ['foo' => 'bar']),
        MongoDB\BSON\Value::TYPE_DOCUMENT,
        'isDocument',
        'getDocument',
    ],
    'binary' => [
        new MongoDB\BSON\Binary('foo'),
        MongoDB\BSON\Value::TYPE_BINARY,
        'isBinary',
        'getBinary',
    ],
    'code' => [
        new MongoDB\BSON\Javascript('foo'),
        MongoDB\BSON\Value::TYPE_CODE,
        'isCode',
        'getCode',
    ],
    'codeWithScope' => [
        new MongoDB\BSON\Javascript('foo', ['foo' => 'bar']),
        MongoDB\BSON\Value::TYPE_CODEWSCOPE,
        'isCode',
        'getCode',
    ],
    'timestamp' => [
        new MongoDB\BSON\Timestamp(1, 2),
        MongoDB\BSON\Value::TYPE_TIMESTAMP,
        'isTimestamp',
        'getTimestamp',
    ],
    'decimal128' => [
        new MongoDB\BSON\Decimal128('1.2'),
        MongoDB\BSON\Value::TYPE_DECIMAL128,
        'isDecimal128',
        'getDecimal128',
    ],
    'minKey' => [
        new MongoDB\BSON\MinKey,
        MongoDB\BSON\Value::TYPE_MINKEY,
        'isMinKey',
        'getMinKey',
    ],
    'maxKey' => [
        new MongoDB\BSON\MaxKey,
        MongoDB\BSON\Value::TYPE_MAXKEY,
        'isMaxKey',
        'getMaxKey',
    ],
    'utcDateTime' => [
        new MongoDB\BSON\UTCDateTime(1234567890),
        MongoDB\BSON\Value::TYPE_DATE_TIME,
        'isDateTime',
        'getDateTime',
    ],
    'regex' => [
        new MongoDB\BSON\Regex('foo'),
        MongoDB\BSON\Value::TYPE_REGEX,
        'isRegex',
        'getRegex',
    ],
    'dbPointer' => [
        MongoDB\BSON\toPHP(MongoDB\BSON\fromJSON('{ "dbref": {"$dbPointer": {"$ref": "phongo.test", "$id" : { "$oid" : "5a2e78accd485d55b4050000" }  }} }'))->dbref,
        MongoDB\BSON\Value::TYPE_DBPOINTER,
        'isDBPointer',
        'getDBPointer',
    ],
    'symbol' => [
        MongoDB\BSON\toPHP(MongoDB\BSON\fromJSON('{ "symbol": {"$symbol": "test"} }'))->symbol,
        MongoDB\BSON\Value::TYPE_SYMBOL,
        'isSymbol',
        'getSymbol',
    ],
    'objectId' => [
        new MongoDB\BSON\ObjectId('644a340b641850b1d0062d50'),
        MongoDB\BSON\Value::TYPE_OID,
        'isObjectId',
        'getObjectId',
    ],
    'undefined' => [
        MongoDB\BSON\toPHP(MongoDB\BSON\fromJSON('{ "undefined": {"$undefined": true} }'))->undefined,
        MongoDB\BSON\Value::TYPE_UNDEFINED,
        'isUndefined',
        'getUndefined',
    ],
];

foreach ($tests as $name => [$value, $type, $successfulIsser, $getter]) {
    printf("Testing %s:\n", $name);
    try {
        $bsonValue = MongoDB\BSON\Value::fromPHP($value);
        var_dump($bsonValue);
        var_dump($bsonValue->getValue());
        var_dump($bsonValue->$getter());
        var_dump($bsonValue->getType() === $type);
        var_dump($bsonValue->$successfulIsser());
    } catch (MongoDB\Driver\Exception\InvalidArgumentException $e) {
        echo $e->getMessage(), "\n";
    }
}

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
Testing null:
object(MongoDB\BSON\Value)#%d (%d) {
  ["type"]=>
  string(4) "null"
  ["value"]=>
  NULL
}
NULL
NULL
bool(true)
bool(true)
Testing int32:
object(MongoDB\BSON\Value)#%d (%d) {
  ["type"]=>
  string(14) "32-bit integer"
  ["value"]=>
  int(2)
}
int(2)
int(2)
bool(true)
bool(true)
Testing int64:
object(MongoDB\BSON\Value)#%d (%d) {
  ["type"]=>
  string(14) "64-bit integer"
  ["value"]=>
  object(MongoDB\BSON\Int64)#%d (%d) {
    ["integer"]=>
    string(1) "2"
  }
}
object(MongoDB\BSON\Int64)#%d (%d) {
  ["integer"]=>
  string(1) "2"
}
object(MongoDB\BSON\Int64)#%d (%d) {
  ["integer"]=>
  string(1) "2"
}
bool(true)
bool(true)
Testing float:
object(MongoDB\BSON\Value)#%d (%d) {
  ["type"]=>
  string(6) "double"
  ["value"]=>
  float(3.14)
}
float(3.14)
float(3.14)
bool(true)
bool(true)
Testing string:
object(MongoDB\BSON\Value)#%d (%d) {
  ["type"]=>
  string(6) "string"
  ["value"]=>
  string(3) "foo"
}
string(3) "foo"
string(3) "foo"
bool(true)
bool(true)
Testing true:
object(MongoDB\BSON\Value)#%d (%d) {
  ["type"]=>
  string(7) "boolean"
  ["value"]=>
  bool(true)
}
bool(true)
bool(true)
bool(true)
bool(true)
Testing false:
object(MongoDB\BSON\Value)#%d (%d) {
  ["type"]=>
  string(7) "boolean"
  ["value"]=>
  bool(false)
}
bool(false)
bool(false)
bool(true)
bool(true)
Testing list:
object(MongoDB\BSON\Value)#%d (%d) {
  ["type"]=>
  string(5) "array"
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
object(MongoDB\BSON\PackedArray)#%d (%d) {
  ["data"]=>
  string(36) "GgAAABAwAAEAAAAQMQACAAAAEDIAAwAAAAA="
}
object(MongoDB\BSON\PackedArray)#%d (%d) {
  ["data"]=>
  string(36) "GgAAABAwAAEAAAAQMQACAAAAEDIAAwAAAAA="
}
bool(true)
bool(true)
Testing packedArray:
object(MongoDB\BSON\Value)#%d (%d) {
  ["type"]=>
  string(5) "array"
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
object(MongoDB\BSON\PackedArray)#%d (%d) {
  ["data"]=>
  string(36) "GgAAABAwAAEAAAAQMQACAAAAEDIAAwAAAAA="
}
object(MongoDB\BSON\PackedArray)#%d (%d) {
  ["data"]=>
  string(36) "GgAAABAwAAEAAAAQMQACAAAAEDIAAwAAAAA="
}
bool(true)
bool(true)
Testing struct:
object(MongoDB\BSON\Value)#%d (%d) {
  ["type"]=>
  string(8) "document"
  ["value"]=>
  object(stdClass)#%d (%d) {
    ["foo"]=>
    string(3) "bar"
  }
}
object(MongoDB\BSON\Document)#%d (%d) {
  ["data"]=>
  string(24) "EgAAAAJmb28ABAAAAGJhcgAA"
}
object(MongoDB\BSON\Document)#%d (%d) {
  ["data"]=>
  string(24) "EgAAAAJmb28ABAAAAGJhcgAA"
}
bool(true)
bool(true)
Testing object:
object(MongoDB\BSON\Value)#%d (%d) {
  ["type"]=>
  string(8) "document"
  ["value"]=>
  object(stdClass)#%d (%d) {
    ["foo"]=>
    string(3) "bar"
  }
}
object(MongoDB\BSON\Document)#%d (%d) {
  ["data"]=>
  string(24) "EgAAAAJmb28ABAAAAGJhcgAA"
}
object(MongoDB\BSON\Document)#%d (%d) {
  ["data"]=>
  string(24) "EgAAAAJmb28ABAAAAGJhcgAA"
}
bool(true)
bool(true)
Testing document:
object(MongoDB\BSON\Value)#%d (%d) {
  ["type"]=>
  string(8) "document"
  ["value"]=>
  object(stdClass)#%d (%d) {
    ["foo"]=>
    string(3) "bar"
  }
}
object(MongoDB\BSON\Document)#%d (%d) {
  ["data"]=>
  string(24) "EgAAAAJmb28ABAAAAGJhcgAA"
}
object(MongoDB\BSON\Document)#%d (%d) {
  ["data"]=>
  string(24) "EgAAAAJmb28ABAAAAGJhcgAA"
}
bool(true)
bool(true)
Testing binary:
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
object(MongoDB\BSON\Binary)#%d (%d) {
  ["data"]=>
  string(3) "foo"
  ["type"]=>
  int(0)
}
object(MongoDB\BSON\Binary)#%d (%d) {
  ["data"]=>
  string(3) "foo"
  ["type"]=>
  int(0)
}
bool(true)
bool(true)
Testing code:
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
object(MongoDB\BSON\Javascript)#%d (%d) {
  ["code"]=>
  string(3) "foo"
  ["scope"]=>
  NULL
}
object(MongoDB\BSON\Javascript)#%d (%d) {
  ["code"]=>
  string(3) "foo"
  ["scope"]=>
  NULL
}
bool(true)
bool(true)
Testing codeWithScope:
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
object(MongoDB\BSON\Javascript)#%d (%d) {
  ["code"]=>
  string(3) "foo"
  ["scope"]=>
  object(stdClass)#%d (%d) {
    ["foo"]=>
    string(3) "bar"
  }
}
object(MongoDB\BSON\Javascript)#%d (%d) {
  ["code"]=>
  string(3) "foo"
  ["scope"]=>
  object(stdClass)#%d (%d) {
    ["foo"]=>
    string(3) "bar"
  }
}
bool(true)
bool(true)
Testing timestamp:
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
object(MongoDB\BSON\Timestamp)#%d (%d) {
  ["increment"]=>
  string(1) "1"
  ["timestamp"]=>
  string(1) "2"
}
object(MongoDB\BSON\Timestamp)#%d (%d) {
  ["increment"]=>
  string(1) "1"
  ["timestamp"]=>
  string(1) "2"
}
bool(true)
bool(true)
Testing decimal128:
object(MongoDB\BSON\Value)#%d (%d) {
  ["type"]=>
  string(10) "Decimal128"
  ["value"]=>
  object(MongoDB\BSON\Decimal128)#%d (%d) {
    ["dec"]=>
    string(3) "1.2"
  }
}
object(MongoDB\BSON\Decimal128)#%d (%d) {
  ["dec"]=>
  string(3) "1.2"
}
object(MongoDB\BSON\Decimal128)#%d (%d) {
  ["dec"]=>
  string(3) "1.2"
}
bool(true)
bool(true)
Testing minKey:
object(MongoDB\BSON\Value)#%d (%d) {
  ["type"]=>
  string(6) "MinKey"
  ["value"]=>
  object(MongoDB\BSON\MinKey)#%d (%d) {
  }
}
object(MongoDB\BSON\MinKey)#%d (%d) {
}
object(MongoDB\BSON\MinKey)#%d (%d) {
}
bool(true)
bool(true)
Testing maxKey:
object(MongoDB\BSON\Value)#%d (%d) {
  ["type"]=>
  string(6) "MaxKey"
  ["value"]=>
  object(MongoDB\BSON\MaxKey)#%d (%d) {
  }
}
object(MongoDB\BSON\MaxKey)#%d (%d) {
}
object(MongoDB\BSON\MaxKey)#%d (%d) {
}
bool(true)
bool(true)
Testing utcDateTime:
object(MongoDB\BSON\Value)#%d (%d) {
  ["type"]=>
  string(11) "UTCDateTime"
  ["value"]=>
  object(MongoDB\BSON\UTCDateTime)#%d (%d) {
    ["milliseconds"]=>
    string(10) "1234567890"
  }
}
object(MongoDB\BSON\UTCDateTime)#%d (%d) {
  ["milliseconds"]=>
  string(10) "1234567890"
}
object(MongoDB\BSON\UTCDateTime)#%d (%d) {
  ["milliseconds"]=>
  string(10) "1234567890"
}
bool(true)
bool(true)
Testing regex:
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
object(MongoDB\BSON\Regex)#%d (%d) {
  ["pattern"]=>
  string(3) "foo"
  ["flags"]=>
  string(0) ""
}
object(MongoDB\BSON\Regex)#%d (%d) {
  ["pattern"]=>
  string(3) "foo"
  ["flags"]=>
  string(0) ""
}
bool(true)
bool(true)
Testing dbPointer:
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
object(MongoDB\BSON\DBPointer)#%d (%d) {
  ["ref"]=>
  string(11) "phongo.test"
  ["id"]=>
  string(24) "5a2e78accd485d55b4050000"
}
object(MongoDB\BSON\DBPointer)#%d (%d) {
  ["ref"]=>
  string(11) "phongo.test"
  ["id"]=>
  string(24) "5a2e78accd485d55b4050000"
}
bool(true)
bool(true)
Testing symbol:
object(MongoDB\BSON\Value)#%d (%d) {
  ["type"]=>
  string(6) "symbol"
  ["value"]=>
  object(MongoDB\BSON\Symbol)#%d (%d) {
    ["symbol"]=>
    string(4) "test"
  }
}
object(MongoDB\BSON\Symbol)#%d (%d) {
  ["symbol"]=>
  string(4) "test"
}
object(MongoDB\BSON\Symbol)#%d (%d) {
  ["symbol"]=>
  string(4) "test"
}
bool(true)
bool(true)
Testing objectId:
object(MongoDB\BSON\Value)#%d (%d) {
  ["type"]=>
  string(8) "ObjectId"
  ["value"]=>
  object(MongoDB\BSON\ObjectId)#%d (%d) {
    ["oid"]=>
    string(24) "644a340b641850b1d0062d50"
  }
}
object(MongoDB\BSON\ObjectId)#%d (%d) {
  ["oid"]=>
  string(24) "644a340b641850b1d0062d50"
}
object(MongoDB\BSON\ObjectId)#%d (%d) {
  ["oid"]=>
  string(24) "644a340b641850b1d0062d50"
}
bool(true)
bool(true)
Testing undefined:
object(MongoDB\BSON\Value)#%d (%d) {
  ["type"]=>
  string(9) "undefined"
  ["value"]=>
  object(MongoDB\BSON\Undefined)#%d (%d) {
  }
}
object(MongoDB\BSON\Undefined)#%d (%d) {
}
object(MongoDB\BSON\Undefined)#%d (%d) {
}
bool(true)
bool(true)
===DONE===
