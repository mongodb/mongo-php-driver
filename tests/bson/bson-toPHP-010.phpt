--TEST--
MongoDB\BSON\toPHP(): Setting fieldPath typemaps for compound types with wildcard keys
--FILE--
<?php

require_once __DIR__ . "/../utils/tools.php";

class MyArrayObject extends ArrayObject implements MongoDB\BSON\Unserializable
{
    function bsonUnserialize(array $data)
    {
        parent::__construct($data, ArrayObject::ARRAY_AS_PROPS);
    }
}

class MyWildcardArrayObject extends ArrayObject implements MongoDB\BSON\Unserializable
{
    function bsonUnserialize(array $data)
    {
        parent::__construct($data, ArrayObject::ARRAY_AS_PROPS);
    }
}

$bson = \MongoDB\BSON\fromPHP( [
    '_id' => 1,
    'array' => [0 => [ 4, 5, 6 ], 1 => [ 7, 8, 9 ]],
    'object' => ['one' => [ 4, 5, 6 ], 'two' => [ 7, 8, 9 ]],
] );

function fetch($bson, $typeMap = []) {
    return \MongoDB\BSON\toPHP($bson, $typeMap);
}


echo "\nSetting 'array.$' path to 'MyWildcardArrayObject'\n";
$document = fetch($bson, ["fieldPaths" => [
    'array.$' => "MyWildcardArrayObject"
]]);
var_dump($document instanceof stdClass);
var_dump(is_array($document->array));
var_dump($document->array[0] instanceof MyWildcardArrayObject);
var_dump($document->array[1] instanceof MyWildcardArrayObject);

echo "\nSetting 'array.1' to 'MyArrayObject' and 'array.$' path to 'MyWildcardArrayObject'\n";
$document = fetch($bson, ["fieldPaths" => [
    'array.1' => "MyArrayObject",
    'array.$' => "MyWildcardArrayObject"
]]);
var_dump($document instanceof stdClass);
var_dump(is_array($document->array));
var_dump($document->array[0] instanceof MyWildcardArrayObject);
var_dump($document->array[1] instanceof MyArrayObject);

echo "\nSetting 'array.$' to 'MyWildcardArrayObject' and 'array.1' path to 'MyArrayObject'\n";
$document = fetch($bson, ["fieldPaths" => [
    'array.$' => "MyWildcardArrayObject",
    'array.1' => "MyArrayObject"
]]);
var_dump($document instanceof stdClass);
var_dump(is_array($document->array));
var_dump($document->array[0] instanceof MyWildcardArrayObject);
var_dump($document->array[1] instanceof MyWildcardArrayObject);


echo "\nSetting 'object.$' path to 'MyWildcardArrayObject'\n";
$document = fetch($bson, ["fieldPaths" => [
    'object.$' => "MyWildcardArrayObject"
]]);
var_dump($document instanceof stdClass);
var_dump(is_object($document->object));
var_dump($document->object->one instanceof MyWildcardArrayObject);
var_dump($document->object->two instanceof MyWildcardArrayObject);

echo "\nSetting 'object.two' to 'MyArrayObject' and 'object.$' path to 'MyWildcardArrayObject'\n";
$document = fetch($bson, ["fieldPaths" => [
    'object.two' => "MyArrayObject",
    'object.$' => "MyWildcardArrayObject"
]]);
var_dump($document instanceof stdClass);
var_dump(is_object($document->object));
var_dump($document->object->one instanceof MyWildcardArrayObject);
var_dump($document->object->two instanceof MyArrayObject);

echo "\nSetting 'object.$' to 'MyWildcardArrayObject' and 'object.one' path to 'MyArrayObject'\n";
$document = fetch($bson, ["fieldPaths" => [
    'object.$' => "MyWildcardArrayObject",
    'object.one' => "MyArrayObject"
]]);
var_dump($document instanceof stdClass);
var_dump(is_object($document->object));
var_dump($document->object->one instanceof MyWildcardArrayObject);
var_dump($document->object->two instanceof MyWildcardArrayObject);
?>
===DONE===
<?php exit(0); ?>
--EXPECT--
Setting 'array.$' path to 'MyWildcardArrayObject'
bool(true)
bool(true)
bool(true)
bool(true)

Setting 'array.1' to 'MyArrayObject' and 'array.$' path to 'MyWildcardArrayObject'
bool(true)
bool(true)
bool(true)
bool(true)

Setting 'array.$' to 'MyWildcardArrayObject' and 'array.1' path to 'MyArrayObject'
bool(true)
bool(true)
bool(true)
bool(true)

Setting 'object.$' path to 'MyWildcardArrayObject'
bool(true)
bool(true)
bool(true)
bool(true)

Setting 'object.two' to 'MyArrayObject' and 'object.$' path to 'MyWildcardArrayObject'
bool(true)
bool(true)
bool(true)
bool(true)

Setting 'object.$' to 'MyWildcardArrayObject' and 'object.one' path to 'MyArrayObject'
bool(true)
bool(true)
bool(true)
bool(true)
===DONE===
