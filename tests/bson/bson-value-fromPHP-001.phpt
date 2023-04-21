--TEST--
MongoDB\BSON\Value::fromPHP()
--FILE--
<?php

require_once __DIR__ . '/../utils/basic.inc';

$tests = [
    'null' => [
        null,
        MongoDB\BSON\Value::TYPE_NULL,
    ],
    'int32' => [
        2,
        MongoDB\BSON\Value::TYPE_INT32,
    ],
    'int64' => [
        2**33,
        MongoDB\BSON\Value::TYPE_INT64,
    ],
    'float' => [
        3.14,
        MongoDB\BSON\Value::TYPE_DOUBLE,
    ],
    'string' => [
        'foo',
        MongoDB\BSON\Value::TYPE_UTF8,
    ],
    'true' => [
        true,
        MongoDB\BSON\Value::TYPE_BOOL,
    ],
    'false' => [
        false,
        MongoDB\BSON\Value::TYPE_BOOL,
    ],
    'list' => [
        [1, 2, 3],
        MongoDB\BSON\Value::TYPE_ARRAY,
    ],
    'struct' => [
        ['foo' => 'bar'],
        MongoDB\BSON\Value::TYPE_DOCUMENT,
    ],
    'object' => [
        (object) ['foo' => 'bar'],
        MongoDB\BSON\Value::TYPE_DOCUMENT,
    ],
];

foreach ($tests as $name => [$value, $type]) {
    printf("Testing %s:\n", $name);
    try {
        $bsonValue = MongoDB\BSON\Value::fromPHP($value);
        var_dump($bsonValue);
        var_dump($bsonValue->getValue());
        var_dump($bsonValue->getType() === $type);
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
bool(true)
Testing int32:
object(MongoDB\BSON\Value)#%d (%d) {
}
int(2)
bool(true)
Testing int64:
object(MongoDB\BSON\Value)#%d (%d) {
}
int(8589934592)
bool(true)
Testing float:
object(MongoDB\BSON\Value)#%d (%d) {
}
float(3.14)
bool(true)
Testing string:
object(MongoDB\BSON\Value)#%d (%d) {
}
string(3) "foo"
bool(true)
Testing true:
object(MongoDB\BSON\Value)#%d (%d) {
}
bool(true)
bool(true)
Testing false:
object(MongoDB\BSON\Value)#%d (%d) {
}
bool(false)
bool(true)
Testing list:
Array and objects are not supported for Value
Testing struct:
Array and objects are not supported for Value
Testing object:
Array and objects are not supported for Value
===DONE===
