--TEST--
MongoDB\Driver\Cursor::setTypeMap(): Setting fieldPath typemaps for compound types with wildcard keys (nested)
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

class MyWildcardArrayObject extends MyArrayObject {};

$manager = new MongoDB\Driver\Manager(URI);

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


echo "\nSetting 'object.$.child1' path to 'MyWildcardArrayObject'\n";
$documents = fetch($manager, ["fieldPaths" => [
    'object.$.child1' => "MyWildcardArrayObject"
]]);
var_dump($documents[0]->object->parent1 instanceof stdClass);
var_dump($documents[0]->object->parent1->child1 instanceof MyWildcardArrayObject);
var_dump(is_array($documents[0]->object->parent1->child2));
var_dump($documents[0]->object->parent2 instanceof stdClass);
var_dump($documents[0]->object->parent2->child1 instanceof MyWildcardArrayObject);
var_dump(is_array($documents[0]->object->parent2->child2));

echo "\nSetting 'object.parent1.$' path to 'MyWildcardArrayObject' and 'object.parent2.child1' to 'MyArrayObject'\n";
$documents = fetch($manager, ["fieldPaths" => [
    'object.parent1.$'      => "MyWildcardArrayObject",
    'object.parent2.child1' => "MyArrayObject",
]]);
var_dump($documents[0]->object->parent1 instanceof stdClass);
var_dump($documents[0]->object->parent1->child1 instanceof MyWildcardArrayObject);
var_dump($documents[0]->object->parent1->child2 instanceof MyWildcardArrayObject);
var_dump($documents[0]->object->parent2 instanceof stdClass);
var_dump($documents[0]->object->parent2->child1 instanceof MyArrayObject);
var_dump(is_array($documents[0]->object->parent2->child2));

echo "\nSetting 'object.parent1.$' path to 'MyWildcardArrayObject' and 'object.$.$' to 'MyArrayObject'\n";
$documents = fetch($manager, ["fieldPaths" => [
    'object.parent1.$'    => "MyWildcardArrayObject",
    'object.$.$'          => "MyArrayObject",
]]);
var_dump($documents[0]->object->parent1 instanceof stdClass);
var_dump($documents[0]->object->parent1->child1 instanceof MyWildcardArrayObject);
var_dump($documents[0]->object->parent1->child2 instanceof MyWildcardArrayObject);
var_dump($documents[0]->object->parent2 instanceof stdClass);
var_dump($documents[0]->object->parent2->child1 instanceof MyArrayObject);
var_dump($documents[0]->object->parent2->child2 instanceof MyArrayObject);

echo "\nSetting 'object.parent1.$' path to 'MyWildcardArrayObject' and 'object.$.child2' to 'MyArrayObject'\n";
$documents = fetch($manager, ["fieldPaths" => [
    'object.parent1.child1' => "MyWildcardArrayObject",
    'object.$.child2'       => "MyArrayObject",
]]);
var_dump($documents[0]->object->parent1 instanceof stdClass);
var_dump($documents[0]->object->parent1->child1 instanceof MyWildcardArrayObject);
var_dump($documents[0]->object->parent1->child2 instanceof MyArrayObject);
var_dump($documents[0]->object->parent2 instanceof stdClass);
var_dump(is_array($documents[0]->object->parent2->child1));
var_dump($documents[0]->object->parent2->child2 instanceof MyArrayObject);

echo "\nSetting 'object.parent1.child2 path to 'MyArrayObject' and 'object.$.$' to 'MyWildcardArrayObject'\n";
$documents = fetch($manager, ["fieldPaths" => [
    'object.parent1.child2' => "MyArrayObject",
    'object.$.$'            => "MyWildcardArrayObject",
]]);
var_dump($documents[0]->object->parent1 instanceof stdClass);
var_dump($documents[0]->object->parent1->child1 instanceof MyWildcardArrayObject);
var_dump($documents[0]->object->parent1->child2 instanceof MyArrayObject);
var_dump($documents[0]->object->parent2 instanceof stdClass);
var_dump($documents[0]->object->parent2->child1 instanceof MyWildcardArrayObject);
var_dump($documents[0]->object->parent2->child2 instanceof MyWildcardArrayObject);
?>
===DONE===
<?php exit(0); ?>
--EXPECT--
Setting 'object.$.child1' path to 'MyWildcardArrayObject'
bool(true)
bool(true)
bool(true)
bool(true)
bool(true)
bool(true)

Setting 'object.parent1.$' path to 'MyWildcardArrayObject' and 'object.parent2.child1' to 'MyArrayObject'
bool(true)
bool(true)
bool(true)
bool(true)
bool(true)
bool(true)

Setting 'object.parent1.$' path to 'MyWildcardArrayObject' and 'object.$.$' to 'MyArrayObject'
bool(true)
bool(true)
bool(true)
bool(true)
bool(true)
bool(true)

Setting 'object.parent1.$' path to 'MyWildcardArrayObject' and 'object.$.child2' to 'MyArrayObject'
bool(true)
bool(true)
bool(true)
bool(true)
bool(true)
bool(true)

Setting 'object.parent1.child2 path to 'MyArrayObject' and 'object.$.$' to 'MyWildcardArrayObject'
bool(true)
bool(true)
bool(true)
bool(true)
bool(true)
bool(true)
===DONE===
