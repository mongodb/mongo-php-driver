--TEST--
MongoDB\Driver\Cursor::setTypeMap(): Setting fieldPath typemaps for compound types with string keys
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
        parent::__construct($data);
    }
}

$manager = new MongoDB\Driver\Manager(URI);

$document = [
    '_id' => 1,
    'array' => [1, 2, 3],
    'object' => ['string' => 'keys', 'for' => 'ever']
];

$bulk = new MongoDB\Driver\BulkWrite();
$bulk->insert($document);
$manager->executeBulkWrite(NS, $bulk);

function fetch($manager, $typemap = []) {
    $cursor = $manager->executeQuery(NS, new MongoDB\Driver\Query([]));
    if ($typemap) {
        $cursor->setTypeMap($typemap);
    }

    return $cursor->toArray();
}


echo "Default\n";
$documents = fetch($manager);
var_dump($documents[0] instanceof stdClass);
var_dump(is_array($documents[0]->array));
var_dump($documents[0]->object instanceof stdClass);

echo "\nSetting 'object' path to 'MyArrayObject'\n";
$documents = fetch($manager, ["fieldPaths" => [
    'object' => "MyArrayObject"
]]);
var_dump($documents[0] instanceof stdClass);
var_dump(is_array($documents[0]->array));
var_dump($documents[0]->object instanceof MyArrayObject);

echo "\nSetting 'object' and 'array' path to 'MyArrayObject'\n";
$documents = fetch($manager, ["fieldPaths" => [
    'object' => "MyArrayObject",
    'array'  => "MyArrayObject",
]]);
var_dump($documents[0] instanceof stdClass);
var_dump($documents[0]->array instanceof MyArrayObject);
var_dump($documents[0]->object instanceof MyArrayObject);
?>
===DONE===
<?php exit(0); ?>
--EXPECT--
Default
bool(true)
bool(true)
bool(true)

Setting 'object' path to 'MyArrayObject'
bool(true)
bool(true)
bool(true)

Setting 'object' and 'array' path to 'MyArrayObject'
bool(true)
bool(true)
bool(true)
===DONE===
