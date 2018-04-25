--TEST--
MongoDB\Driver\Cursor::toArray() respects type map
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

$bulk = new MongoDB\Driver\BulkWrite();
$bulk->insert(array('_id' => 1, 'x' => array(1, 2, 3)));
$bulk->insert(array('_id' => 2, 'x' => array(4, 5, 6)));
$manager->executeBulkWrite(NS, $bulk);

$cursor = $manager->executeQuery(NS, new MongoDB\Driver\Query(array('x' => 1)));
$cursor->setTypeMap(array("array" => "MyArrayObject"));

$documents = $cursor->toArray();

var_dump($documents[0]->x instanceof MyArrayObject);

?>
===DONE===
<?php exit(0); ?>
--EXPECT--
bool(true)
===DONE===
