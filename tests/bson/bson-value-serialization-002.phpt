--TEST--
MongoDB\BSON\Value serialization (__serialize and __unserialize)
--SKIPIF--
<?php require __DIR__ . "/../utils/basic-skipif.inc"; ?>
<?php skip_if_php_version('<', '7.4.0'); ?>
--FILE--
<?php

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

    var_dump($bsonValue = MongoDB\BSON\Value::fromPHP($value));
    var_dump($s = serialize($bsonValue));
    var_dump(unserialize($s));
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
string(60) "O:18:"MongoDB\BSON\Value":2:{s:4:"type";i:10;s:5:"value";N;}"
object(MongoDB\BSON\Value)#%d (%d) {
  ["type"]=>
  string(4) "null"
  ["value"]=>
  NULL
}
Testing int32:
object(MongoDB\BSON\Value)#%d (%d) {
  ["type"]=>
  string(14) "32-bit integer"
  ["value"]=>
  int(2)
}
string(62) "O:18:"MongoDB\BSON\Value":2:{s:4:"type";i:16;s:5:"value";i:2;}"
object(MongoDB\BSON\Value)#%d (%d) {
  ["type"]=>
  string(14) "32-bit integer"
  ["value"]=>
  int(2)
}
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
string(110) "O:18:"MongoDB\BSON\Value":2:{s:4:"type";i:18;s:5:"value";O:18:"MongoDB\BSON\Int64":1:{s:7:"integer";s:1:"2";}}"
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
object(MongoDB\BSON\Value)#%d (%d) {
  ["type"]=>
  string(6) "double"
  ["value"]=>
  float(3.14)
}
string(64) "O:18:"MongoDB\BSON\Value":2:{s:4:"type";i:1;s:5:"value";d:3.14;}"
object(MongoDB\BSON\Value)#%d (%d) {
  ["type"]=>
  string(6) "double"
  ["value"]=>
  float(3.14)
}
Testing string:
object(MongoDB\BSON\Value)#%d (%d) {
  ["type"]=>
  string(6) "string"
  ["value"]=>
  string(3) "foo"
}
string(67) "O:18:"MongoDB\BSON\Value":2:{s:4:"type";i:2;s:5:"value";s:3:"foo";}"
object(MongoDB\BSON\Value)#%d (%d) {
  ["type"]=>
  string(6) "string"
  ["value"]=>
  string(3) "foo"
}
Testing true:
object(MongoDB\BSON\Value)#%d (%d) {
  ["type"]=>
  string(7) "boolean"
  ["value"]=>
  bool(true)
}
string(61) "O:18:"MongoDB\BSON\Value":2:{s:4:"type";i:8;s:5:"value";b:1;}"
object(MongoDB\BSON\Value)#%d (%d) {
  ["type"]=>
  string(7) "boolean"
  ["value"]=>
  bool(true)
}
Testing false:
object(MongoDB\BSON\Value)#%d (%d) {
  ["type"]=>
  string(7) "boolean"
  ["value"]=>
  bool(false)
}
string(61) "O:18:"MongoDB\BSON\Value":2:{s:4:"type";i:8;s:5:"value";b:0;}"
object(MongoDB\BSON\Value)#%d (%d) {
  ["type"]=>
  string(7) "boolean"
  ["value"]=>
  bool(false)
}
Testing list:
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
string(148) "O:18:"MongoDB\BSON\Value":2:{s:4:"type";i:4;s:5:"value";O:24:"MongoDB\BSON\PackedArray":1:{s:4:"data";s:36:"GgAAABAwAAEAAAAQMQACAAAAEDIAAwAAAAA=";}}"
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
string(148) "O:18:"MongoDB\BSON\Value":2:{s:4:"type";i:4;s:5:"value";O:24:"MongoDB\BSON\PackedArray":1:{s:4:"data";s:36:"GgAAABAwAAEAAAAQMQACAAAAEDIAAwAAAAA=";}}"
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
string(133) "O:18:"MongoDB\BSON\Value":2:{s:4:"type";i:3;s:5:"value";O:21:"MongoDB\BSON\Document":1:{s:4:"data";s:24:"EgAAAAJmb28ABAAAAGJhcgAA";}}"
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
string(133) "O:18:"MongoDB\BSON\Value":2:{s:4:"type";i:3;s:5:"value";O:21:"MongoDB\BSON\Document":1:{s:4:"data";s:24:"EgAAAAJmb28ABAAAAGJhcgAA";}}"
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
string(133) "O:18:"MongoDB\BSON\Value":2:{s:4:"type";i:3;s:5:"value";O:21:"MongoDB\BSON\Document":1:{s:4:"data";s:24:"EgAAAAJmb28ABAAAAGJhcgAA";}}"
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
string(124) "O:18:"MongoDB\BSON\Value":2:{s:4:"type";i:5;s:5:"value";O:19:"MongoDB\BSON\Binary":2:{s:4:"data";s:3:"foo";s:4:"type";i:0;}}"
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
string(128) "O:18:"MongoDB\BSON\Value":2:{s:4:"type";i:13;s:5:"value";O:23:"MongoDB\BSON\Javascript":2:{s:4:"code";s:3:"foo";s:5:"scope";N;}}"
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
string(165) "O:18:"MongoDB\BSON\Value":2:{s:4:"type";i:15;s:5:"value";O:23:"MongoDB\BSON\Javascript":2:{s:4:"code";s:3:"foo";s:5:"scope";O:8:"stdClass":1:{s:3:"foo";s:3:"bar";}}}"
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
string(140) "O:18:"MongoDB\BSON\Value":2:{s:4:"type";i:17;s:5:"value";O:22:"MongoDB\BSON\Timestamp":2:{s:9:"increment";s:1:"1";s:9:"timestamp";s:1:"2";}}"
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
object(MongoDB\BSON\Value)#%d (%d) {
  ["type"]=>
  string(10) "Decimal128"
  ["value"]=>
  object(MongoDB\BSON\Decimal128)#%d (%d) {
    ["dec"]=>
    string(3) "1.2"
  }
}
string(113) "O:18:"MongoDB\BSON\Value":2:{s:4:"type";i:19;s:5:"value";O:23:"MongoDB\BSON\Decimal128":1:{s:3:"dec";s:3:"1.2";}}"
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
object(MongoDB\BSON\Value)#%d (%d) {
  ["type"]=>
  string(6) "MinKey"
  ["value"]=>
  object(MongoDB\BSON\MinKey)#%d (%d) {
  }
}
string(90) "O:18:"MongoDB\BSON\Value":2:{s:4:"type";i:255;s:5:"value";O:19:"MongoDB\BSON\MinKey":0:{}}"
object(MongoDB\BSON\Value)#%d (%d) {
  ["type"]=>
  string(6) "MinKey"
  ["value"]=>
  object(MongoDB\BSON\MinKey)#%d (%d) {
  }
}
Testing maxKey:
object(MongoDB\BSON\Value)#%d (%d) {
  ["type"]=>
  string(6) "MaxKey"
  ["value"]=>
  object(MongoDB\BSON\MaxKey)#%d (%d) {
  }
}
string(90) "O:18:"MongoDB\BSON\Value":2:{s:4:"type";i:127;s:5:"value";O:19:"MongoDB\BSON\MaxKey":0:{}}"
object(MongoDB\BSON\Value)#%d (%d) {
  ["type"]=>
  string(6) "MaxKey"
  ["value"]=>
  object(MongoDB\BSON\MaxKey)#%d (%d) {
  }
}
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
string(131) "O:18:"MongoDB\BSON\Value":2:{s:4:"type";i:9;s:5:"value";O:24:"MongoDB\BSON\UTCDateTime":1:{s:12:"milliseconds";s:10:"1234567890";}}"
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
string(131) "O:18:"MongoDB\BSON\Value":2:{s:4:"type";i:11;s:5:"value";O:18:"MongoDB\BSON\Regex":2:{s:7:"pattern";s:3:"foo";s:5:"flags";s:0:"";}}"
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
string(162) "O:18:"MongoDB\BSON\Value":2:{s:4:"type";i:12;s:5:"value";O:22:"MongoDB\BSON\DBPointer":2:{s:3:"ref";s:11:"phongo.test";s:2:"id";s:24:"5a2e78accd485d55b4050000";}}"
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
object(MongoDB\BSON\Value)#%d (%d) {
  ["type"]=>
  string(6) "symbol"
  ["value"]=>
  object(MongoDB\BSON\Symbol)#%d (%d) {
    ["symbol"]=>
    string(4) "test"
  }
}
string(113) "O:18:"MongoDB\BSON\Value":2:{s:4:"type";i:14;s:5:"value";O:19:"MongoDB\BSON\Symbol":1:{s:6:"symbol";s:4:"test";}}"
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
object(MongoDB\BSON\Value)#%d (%d) {
  ["type"]=>
  string(8) "ObjectId"
  ["value"]=>
  object(MongoDB\BSON\ObjectId)#%d (%d) {
    ["oid"]=>
    string(24) "644a340b641850b1d0062d50"
  }
}
string(132) "O:18:"MongoDB\BSON\Value":2:{s:4:"type";i:7;s:5:"value";O:21:"MongoDB\BSON\ObjectId":1:{s:3:"oid";s:24:"644a340b641850b1d0062d50";}}"
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
object(MongoDB\BSON\Value)#%d (%d) {
  ["type"]=>
  string(9) "undefined"
  ["value"]=>
  object(MongoDB\BSON\Undefined)#%d (%d) {
  }
}
string(91) "O:18:"MongoDB\BSON\Value":2:{s:4:"type";i:6;s:5:"value";O:22:"MongoDB\BSON\Undefined":0:{}}"
object(MongoDB\BSON\Value)#%d (%d) {
  ["type"]=>
  string(9) "undefined"
  ["value"]=>
  object(MongoDB\BSON\Undefined)#%d (%d) {
  }
}
===DONE===
