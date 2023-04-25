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
    // Int64 omitted, tested separately to account for 32-bit platforms
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
}
NULL
NULL
bool(true)
bool(true)
Testing int32:
object(MongoDB\BSON\Value)#%d (%d) {
}
int(2)
int(2)
bool(true)
bool(true)
Testing float:
object(MongoDB\BSON\Value)#%d (%d) {
}
float(3.14)
float(3.14)
bool(true)
bool(true)
Testing string:
object(MongoDB\BSON\Value)#%d (%d) {
}
string(3) "foo"
string(3) "foo"
bool(true)
bool(true)
Testing true:
object(MongoDB\BSON\Value)#%d (%d) {
}
bool(true)
bool(true)
bool(true)
bool(true)
Testing false:
object(MongoDB\BSON\Value)#%d (%d) {
}
bool(false)
bool(false)
bool(true)
bool(true)
Testing list:
object(MongoDB\BSON\Value)#%d (%d) {
}
array(3) {
  [0]=>
  int(1)
  [1]=>
  int(2)
  [2]=>
  int(3)
}
object(MongoDB\BSON\PackedArray)#%d (%d) {
  ["data"]=>
  string(36) "GgAAABAwAAEAAAAQMQACAAAAEDIAAwAAAAA="
}
bool(true)
bool(true)
Testing struct:
object(MongoDB\BSON\Value)#%d (%d) {
}
object(stdClass)#%d (%d) {
  ["foo"]=>
  string(3) "bar"
}
object(MongoDB\BSON\Document)#%d (%d) {
  ["data"]=>
  string(24) "EgAAAAJmb28ABAAAAGJhcgAA"
}
bool(true)
bool(true)
Testing object:
object(MongoDB\BSON\Value)#%d (%d) {
}
object(stdClass)#%d (%d) {
  ["foo"]=>
  string(3) "bar"
}
object(MongoDB\BSON\Document)#%d (%d) {
  ["data"]=>
  string(24) "EgAAAAJmb28ABAAAAGJhcgAA"
}
bool(true)
bool(true)
===DONE===
