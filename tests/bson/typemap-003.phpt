--TEST--
MongoDB\Driver\Cursor::setTypeMap(): Setting and replacing typemaps
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
        parent::__construct($data, \ArrayObject::ARRAY_AS_PROPS);
    }
}

class MyProperties extends MyArrayObject
{
}

$manager = new MongoDB\Driver\Manager(URI);

$document1 = [
    '_id' => 1,
    'array' => [1, 2, 3],
    'object' => ['string' => ['sleutels', 'keys'] ]
];
$document2 = [
    '_id' => 2,
    'array' => [4, 5, 6],
    'object' => ['associative' => ['elementen', 'elements' ]]
];

$bulk = new MongoDB\Driver\BulkWrite();
$bulk->insert($document1);
$bulk->insert($document2);
$manager->executeBulkWrite(NS, $bulk);

$typemap1 = ["fieldPaths" => [
    'object.string' => "MyArrayObject",
    'object'        => "MyArrayObject",
]];
$typemap2 = ["fieldPaths" => [
    'object.associative' => "MyProperties",
    'object'             => "MyArrayObject",
]];

$cursor = $manager->executeQuery(NS, new MongoDB\Driver\Query([]));
$cursor->setTypeMap($typemap1);
$cursor->setTypeMap($typemap2);
    
$documents = $cursor->toArray();

var_dump($documents[0] instanceof stdClass);
var_dump(is_array($documents[0]->array));
var_dump($documents[0]->object instanceof MyArrayObject);
var_dump(is_array($documents[0]->object['string']));
var_dump(is_array($documents[0]->object->string));

var_dump($documents[1] instanceof stdClass);
var_dump(is_array($documents[1]->array));
var_dump($documents[1]->object instanceof MyArrayObject);
var_dump($documents[1]->object['associative'] instanceof MyProperties);
var_dump($documents[1]->object->associative instanceof MyProperties);
?>
===DONE===
<?php exit(0); ?>
--EXPECT--
bool(true)
bool(true)
bool(true)
bool(true)
bool(true)
bool(true)
bool(true)
bool(true)
bool(true)
bool(true)
===DONE===
