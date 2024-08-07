--TEST--
MongoDB\BSON\toPHP(): Setting fieldPath typemaps for compound types with string keys
--FILE--
<?php

require_once __DIR__ . "/../utils/basic.inc";

class MyArrayObject extends ArrayObject implements MongoDB\BSON\Unserializable
{
    public function bsonUnserialize(array $data): void
    {
        parent::__construct($data);
    }
}

$bson = \MongoDB\BSON\fromPHP( [
    '_id' => 1,
    'array' => [1, 2, 3],
    'object' => ['string' => 'keys', 'for' => 'ever']
] );

function fetch($bson, $typeMap = []) {
    return \MongoDB\BSON\toPHP($bson, $typeMap);
}


echo "Default\n";
$document = fetch($bson);
var_dump($document instanceof stdClass);
var_dump(is_array($document->array));
var_dump($document->object instanceof stdClass);

echo "\nSetting 'object' path to 'MyArrayObject'\n";
$document = fetch($bson, ["fieldPaths" => [
    'object' => "MyArrayObject"
]]);
var_dump($document instanceof stdClass);
var_dump(is_array($document->array));
var_dump($document->object instanceof MyArrayObject);

echo "\nSetting 'object' and 'array' path to 'MyArrayObject'\n";
$document = fetch($bson, ["fieldPaths" => [
    'object' => "MyArrayObject",
    'array'  => "MyArrayObject",
]]);
var_dump($document instanceof stdClass);
var_dump($document->array instanceof MyArrayObject);
var_dump($document->object instanceof MyArrayObject);
?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
Deprecated: Function MongoDB\BSON\fromPHP() is deprecated in %s
Default

Deprecated: Function MongoDB\BSON\toPHP() is deprecated in %s
bool(true)
bool(true)
bool(true)

Setting 'object' path to 'MyArrayObject'

Deprecated: Function MongoDB\BSON\toPHP() is deprecated in %s
bool(true)
bool(true)
bool(true)

Setting 'object' and 'array' path to 'MyArrayObject'

Deprecated: Function MongoDB\BSON\toPHP() is deprecated in %s
bool(true)
bool(true)
bool(true)
===DONE===
