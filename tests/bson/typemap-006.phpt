--TEST--
MongoDB\Driver\Cursor::setTypeMap(): Setting fieldPath typemaps for compound types with wildcard keys
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

class MyWildcardArrayObject extends ArrayObject implements MongoDB\BSON\Unserializable
{
    function bsonUnserialize(array $data)
    {
        parent::__construct($data, ArrayObject::ARRAY_AS_PROPS);
    }
}

$manager = new MongoDB\Driver\Manager(URI);

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


echo "\nSetting 'array.$' path to 'MyWildcardArrayObject'\n";
$documents = fetch($manager, ["fieldPaths" => [
    'array.$' => "MyWildcardArrayObject"
]]);
var_dump($documents[0] instanceof stdClass);
var_dump(is_array($documents[0]->array));
var_dump($documents[0]->array[0] instanceof MyWildcardArrayObject);
var_dump($documents[0]->array[1] instanceof MyWildcardArrayObject);

echo "\nSetting 'array.1' to 'MyArrayObject' and 'array.$' path to 'MyWildcardArrayObject'\n";
$documents = fetch($manager, ["fieldPaths" => [
    'array.1' => "MyArrayObject",
    'array.$' => "MyWildcardArrayObject"
]]);
var_dump($documents[0] instanceof stdClass);
var_dump(is_array($documents[0]->array));
var_dump($documents[0]->array[0] instanceof MyWildcardArrayObject);
var_dump($documents[0]->array[1] instanceof MyArrayObject);

echo "\nSetting 'array.$' to 'MyWildcardArrayObject' and 'array.1' path to 'MyArrayObject'\n";
$documents = fetch($manager, ["fieldPaths" => [
    'array.$' => "MyWildcardArrayObject",
    'array.1' => "MyArrayObject"
]]);
var_dump($documents[0] instanceof stdClass);
var_dump(is_array($documents[0]->array));
var_dump($documents[0]->array[0] instanceof MyWildcardArrayObject);
var_dump($documents[0]->array[1] instanceof MyWildcardArrayObject);


echo "\nSetting 'object.$' path to 'MyWildcardArrayObject'\n";
$documents = fetch($manager, ["fieldPaths" => [
    'object.$' => "MyWildcardArrayObject"
]]);
var_dump($documents[0] instanceof stdClass);
var_dump(is_object($documents[0]->object));
var_dump($documents[0]->object->one instanceof MyWildcardArrayObject);
var_dump($documents[0]->object->two instanceof MyWildcardArrayObject);

echo "\nSetting 'object.two' to 'MyArrayObject' and 'object.$' path to 'MyWildcardArrayObject'\n";
$documents = fetch($manager, ["fieldPaths" => [
    'object.two' => "MyArrayObject",
    'object.$' => "MyWildcardArrayObject"
]]);
var_dump($documents[0] instanceof stdClass);
var_dump(is_object($documents[0]->object));
var_dump($documents[0]->object->one instanceof MyWildcardArrayObject);
var_dump($documents[0]->object->two instanceof MyArrayObject);

echo "\nSetting 'object.$' to 'MyWildcardArrayObject' and 'object.one' path to 'MyArrayObject'\n";
$documents = fetch($manager, ["fieldPaths" => [
    'object.$' => "MyWildcardArrayObject",
    'object.one' => "MyArrayObject"
]]);
var_dump($documents[0] instanceof stdClass);
var_dump(is_object($documents[0]->object));
var_dump($documents[0]->object->one instanceof MyWildcardArrayObject);
var_dump($documents[0]->object->two instanceof MyWildcardArrayObject);
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
