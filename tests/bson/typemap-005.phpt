--TEST--
MongoDB\Driver\Cursor::setTypeMap(): Setting fieldPath typemaps for compound types with numerical keys
--SKIPIF--
<?php require __DIR__ . "/../utils/basic-skipif.inc"; ?>
<?php skip_if_not_live(); ?>
<?php skip_if_not_clean(); ?>
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

$manager = new MongoDB\Driver\Manager(URI);

$document = [
    '_id' => 1,
    'array0' => [0 => [ 4, 5, 6 ], 1 => [ 7, 8, 9 ]],
    'array1' => [1 => [ 4, 5, 6 ], 2 => [ 7, 8, 9 ]],
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


echo "Default\n";
$documents = fetch($manager);
var_dump($documents[0] instanceof stdClass);
var_dump(is_array($documents[0]->array0));
var_dump(is_object($documents[0]->array1));
var_dump($documents[0]->array1 instanceof stdClass);

echo "\nSetting 'array0' path to 'MyArrayObject'\n";
$documents = fetch($manager, ["fieldPaths" => [
    'array0' => "MyArrayObject"
]]);
var_dump($documents[0] instanceof stdClass);
var_dump(is_object($documents[0]->array0));
var_dump($documents[0]->array0 instanceof MyArrayObject);

echo "\nSetting 'array0.1' path to 'MyArrayObject'\n";
$documents = fetch($manager, ["fieldPaths" => [
    'array0.1'  => "MyArrayObject",
]]);
var_dump($documents[0] instanceof stdClass);
var_dump(is_array($documents[0]->array0));
var_dump(is_array($documents[0]->array0[0]));
var_dump($documents[0]->array0[1] instanceof MyArrayObject);

echo "\nSetting 'array1.1' path to 'MyArrayObject'\n";
$documents = fetch($manager, ["fieldPaths" => [
    'array1.1'  => "MyArrayObject",
]]);
var_dump($documents[0] instanceof stdClass);
var_dump(is_object($documents[0]->array1));
var_dump($documents[0]->array1 instanceof stdClass);
$a = ((array) $documents[0]->array1);
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
