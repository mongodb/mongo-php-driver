--TEST--
MongoDB\Driver\Cursor::setTypeMap(): Setting fieldPath typemaps for compound types with wild card keys (nested)
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
    'object' => [
        'parent1' => [
            'child1' => [ 1, 2, 3 ],
            'child2' => [ 4, 5, 6 ],
        ],
        'parent2' => [
            'child1' => [  7,  8,  9 ],
            'child2' => [ 10, 11, 12 ],
        ],
    ],
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


echo "\nSetting 'object.$.child1' path to 'MyWildCardArrayObject'\n";
$documents = fetch($manager, ["fieldPaths" => [
    'object.$.child1' => "MyWildCardArrayObject"
]]);
var_dump($documents[0]->object->parent1 instanceof stdClass);
var_dump($documents[0]->object->parent1->child1 instanceof MyWildCardArrayObject);
var_dump(is_array($documents[0]->object->parent1->child2));
var_dump($documents[0]->object->parent2 instanceof stdClass);
var_dump($documents[0]->object->parent2->child1 instanceof MyWildCardArrayObject);
var_dump(is_array($documents[0]->object->parent2->child2));

echo "\nSetting 'object.parent1.$' path to 'MyWildCardArrayObject' and 'object.parent2.child1' to 'MyArrayObject'\n";
$documents = fetch($manager, ["fieldPaths" => [
    'object.parent1.$'      => "MyWildCardArrayObject",
    'object.parent2.child1' => "MyArrayObject",
]]);
var_dump($documents[0]->object->parent1 instanceof stdClass);
var_dump($documents[0]->object->parent1->child1 instanceof MyWildCardArrayObject);
var_dump($documents[0]->object->parent1->child2 instanceof MyWildCardArrayObject);
var_dump($documents[0]->object->parent2 instanceof stdClass);
var_dump($documents[0]->object->parent2->child1 instanceof MyArrayObject);
var_dump(is_array($documents[0]->object->parent2->child2));

echo "\nSetting 'object.parent1.$' path to 'MyWildCardArrayObject' and 'object.$.$' to 'MyArrayObject'\n";
$documents = fetch($manager, ["fieldPaths" => [
    'object.parent1.$'    => "MyWildCardArrayObject",
    'object.$.$'          => "MyArrayObject",
]]);
var_dump($documents[0]->object->parent1 instanceof stdClass);
var_dump($documents[0]->object->parent1->child1 instanceof MyWildCardArrayObject);
var_dump($documents[0]->object->parent1->child2 instanceof MyWildCardArrayObject);
var_dump($documents[0]->object->parent2 instanceof stdClass);
var_dump($documents[0]->object->parent2->child1 instanceof MyArrayObject);
var_dump($documents[0]->object->parent2->child2 instanceof MyArrayObject);

echo "\nSetting 'object.parent1.$' path to 'MyWildCardArrayObject' and 'object.$.child2' to 'MyArrayObject'\n";
$documents = fetch($manager, ["fieldPaths" => [
    'object.parent1.child1' => "MyWildCardArrayObject",
    'object.$.child2'       => "MyArrayObject",
]]);
var_dump($documents[0]->object->parent1 instanceof stdClass);
var_dump($documents[0]->object->parent1->child1 instanceof MyWildCardArrayObject);
var_dump($documents[0]->object->parent1->child2 instanceof MyArrayObject);
var_dump($documents[0]->object->parent2 instanceof stdClass);
var_dump(is_array($documents[0]->object->parent2->child1));
var_dump($documents[0]->object->parent2->child2 instanceof MyArrayObject);
?>
===DONE===
<?php exit(0); ?>
--EXPECT--
Setting 'object.$.child1' path to 'MyWildCardArrayObject'
bool(true)
bool(true)
bool(true)
bool(true)
bool(true)
bool(true)

Setting 'object.parent1.$' path to 'MyWildCardArrayObject' and 'object.parent2.child1' to 'MyArrayObject'
bool(true)
bool(true)
bool(true)
bool(true)
bool(true)
bool(true)

Setting 'object.parent1.$' path to 'MyWildCardArrayObject' and 'object.$.$' to 'MyArrayObject'
bool(true)
bool(true)
bool(true)
bool(true)
bool(true)
bool(true)

Setting 'object.parent1.$' path to 'MyWildCardArrayObject' and 'object.$.child2' to 'MyArrayObject'
bool(true)
bool(true)
bool(true)
bool(true)
bool(true)
bool(true)
===DONE===
