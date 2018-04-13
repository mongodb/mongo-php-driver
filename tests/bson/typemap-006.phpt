--TEST--
MongoDB\Driver\Cursor::setTypeMap(): Setting fieldPath typemaps for compound types with wild card keys
--SKIPIF--
<?php require __DIR__ . "/../utils/basic-skipif.inc"; ?>
<?php NEEDS('STANDALONE'); CLEANUP(STANDALONE); ?>
--FILE--
<?php

require_once __DIR__ . "/../utils/basic.inc";

class MyArrayObject extends ArrayObject implements MongoDB\BSON\Unserializable
{
    function bsonUnserialize(array $data)
    {
        parent::__construct($data, ArrayObject::ARRAY_AS_PROPS);
    }
}

class MyWildCardArrayObject extends MyArrayObject {};

$manager = new MongoDB\Driver\Manager(STANDALONE);

$document = [
    '_id' => 1,
    'array' => [0 => [ 4, 5, 6 ], 1 => [ 7, 8, 9 ]],
    'object' => ['one' => [ 4, 5, 6 ], 'two' => [ 7, 8, 9 ]],
];

$bulk = new MongoDB\Driver\BulkWrite();
$bulk->insert($document);
$manager->executeBulkWrite(NS, $bulk);

function fetch($manager, $typemap = []) {
    $cursor = $manager->executeQuery(NS, new MongoDB\Driver\Query([]));
    if ($typemap) {
        $cursor->setTypeMap($typemap);
    }

    $documents = $cursor->toArray();
    return $documents;
}


echo "\nSetting 'array.$' path to 'MyWildCardArrayObject'\n";
$documents = fetch($manager, ["fieldPaths" => [
    'array.$' => "MyWildCardArrayObject"
]]);
var_dump($documents[0] instanceof stdClass);
var_dump(is_array($documents[0]->array));
var_dump($documents[0]->array[0] instanceof MyWildCardArrayObject);
var_dump($documents[0]->array[1] instanceof MyWildCardArrayObject);

echo "\nSetting 'array.1' to 'MyArrayObject' and 'array.$' path to 'MyWildCardArrayObject'\n";
$documents = fetch($manager, ["fieldPaths" => [
    'array.1' => "MyArrayObject",
    'array.$' => "MyWildCardArrayObject"
]]);
var_dump($documents[0] instanceof stdClass);
var_dump(is_array($documents[0]->array));
var_dump($documents[0]->array[0] instanceof MyWildCardArrayObject);
var_dump($documents[0]->array[1] instanceof MyArrayObject);

echo "\nSetting 'array.$' to 'MyWildCardArrayObject' and 'array.1' path to 'MyArrayObject'\n";
$documents = fetch($manager, ["fieldPaths" => [
    'array.$' => "MyWildCardArrayObject",
    'array.1' => "MyArrayObject"
]]);
var_dump($documents[0] instanceof stdClass);
var_dump(is_array($documents[0]->array));
var_dump($documents[0]->array[0] instanceof MyWildCardArrayObject);
var_dump($documents[0]->array[1] instanceof MyArrayObject);


echo "\nSetting 'object.$' path to 'MyWildCardArrayObject'\n";
$documents = fetch($manager, ["fieldPaths" => [
    'object.$' => "MyWildCardArrayObject"
]]);
var_dump($documents[0] instanceof stdClass);
var_dump(is_object($documents[0]->object));
var_dump($documents[0]->object->one instanceof MyWildCardArrayObject);
var_dump($documents[0]->object->two instanceof MyWildCardArrayObject);

echo "\nSetting 'object.two' to 'MyArrayObject' and 'object.$' path to 'MyWildCardArrayObject'\n";
$documents = fetch($manager, ["fieldPaths" => [
    'object.two' => "MyArrayObject",
    'object.$' => "MyWildCardArrayObject"
]]);
var_dump($documents[0] instanceof stdClass);
var_dump(is_object($documents[0]->object));
var_dump($documents[0]->object->one instanceof MyWildCardArrayObject);
var_dump($documents[0]->object->two instanceof MyArrayObject);

echo "\nSetting 'object.$' to 'MyWildCardArrayObject' and 'object.one' path to 'MyArrayObject'\n";
$documents = fetch($manager, ["fieldPaths" => [
    'object.$' => "MyWildCardArrayObject",
    'object.one' => "MyArrayObject"
]]);
var_dump($documents[0] instanceof stdClass);
var_dump(is_object($documents[0]->object));
var_dump($documents[0]->object->one instanceof MyArrayObject);
var_dump($documents[0]->object->two instanceof MyWildCardArrayObject);
?>
===DONE===
<?php exit(0); ?>
--EXPECT--
Setting 'array.$' path to 'MyWildCardArrayObject'
bool(true)
bool(true)
bool(true)
bool(true)

Setting 'array.1' to 'MyArrayObject' and 'array.$' path to 'MyWildCardArrayObject'
bool(true)
bool(true)
bool(true)
bool(true)

Setting 'array.$' to 'MyWildCardArrayObject' and 'array.1' path to 'MyArrayObject'
bool(true)
bool(true)
bool(true)
bool(true)

Setting 'object.$' path to 'MyWildCardArrayObject'
bool(true)
bool(true)
bool(true)
bool(true)

Setting 'object.two' to 'MyArrayObject' and 'object.$' path to 'MyWildCardArrayObject'
bool(true)
bool(true)
bool(true)
bool(true)

Setting 'object.$' to 'MyWildCardArrayObject' and 'object.one' path to 'MyArrayObject'
bool(true)
bool(true)
bool(true)
bool(true)
===DONE===
