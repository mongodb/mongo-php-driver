--TEST--
MongoDB\BSON\toPHP(): Setting fieldPath typemaps for compound types with numerical keys
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

$bson = \MongoDB\BSON\fromPHP( [
    '_id' => 1,
    'array0' => [0 => [ 4, 5, 6 ], 1 => [ 7, 8, 9 ]],
    'array1' => [1 => [ 4, 5, 6 ], 2 => [ 7, 8, 9 ]],
] );

function fetch($bson, $typeMap = []) {
    return \MongoDB\BSON\toPHP($bson, $typeMap);
}


echo "Default\n";
$document = fetch($bson);
var_dump($document instanceof stdClass);
var_dump(is_array($document->array0));
var_dump(is_object($document->array1));
var_dump($document->array1 instanceof stdClass);

echo "\nSetting 'array0' path to 'MyArrayObject'\n";
$document = fetch($bson, ["fieldPaths" => [
    'array0' => "MyArrayObject"
]]);
var_dump($document instanceof stdClass);
var_dump(is_object($document->array0));
var_dump($document->array0 instanceof MyArrayObject);

echo "\nSetting 'array0.1' path to 'MyArrayObject'\n";
$document = fetch($bson, ["fieldPaths" => [
    'array0.1'  => "MyArrayObject",
]]);
var_dump($document instanceof stdClass);
var_dump(is_array($document->array0));
var_dump(is_array($document->array0[0]));
var_dump($document->array0[1] instanceof MyArrayObject);

echo "\nSetting 'array1.1' path to 'MyArrayObject'\n";
$document = fetch($bson, ["fieldPaths" => [
    'array1.1'  => "MyArrayObject",
]]);
var_dump($document instanceof stdClass);
var_dump(is_object($document->array1));
var_dump($document->array1 instanceof stdClass);
$a = ((array) $document->array1);
var_dump($a[1] instanceof MyArrayObject);
var_dump(is_array($a[2]));
?>
===DONE===
<?php exit(0); ?>
--EXPECT--
Default
bool(true)
bool(true)
bool(true)
bool(true)

Setting 'array0' path to 'MyArrayObject'
bool(true)
bool(true)
bool(true)

Setting 'array0.1' path to 'MyArrayObject'
bool(true)
bool(true)
bool(true)
bool(true)

Setting 'array1.1' path to 'MyArrayObject'
bool(true)
bool(true)
bool(true)
bool(true)
bool(true)
===DONE===
