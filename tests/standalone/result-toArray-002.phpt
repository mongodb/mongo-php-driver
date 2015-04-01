--TEST--
MongoDB\Driver\Result::toArray() respects type map
--SKIPIF--
<?php require __DIR__ . "/../utils/basic-skipif.inc"; CLEANUP(STANDALONE) ?>
--FILE--
<?php
require_once __DIR__ . "/../utils/basic.inc";

class MyArrayObject extends ArrayObject implements BSON\Unserializable
{
    function bsonUnserialize(array $data)
    {
        parent::__construct($data);
    }
}

$manager = new MongoDB\Driver\Manager(STANDALONE);

$manager->executeInsert(NS, array('_id' => 1, 'x' => array(1, 2, 3)));
$manager->executeInsert(NS, array('_id' => 2, 'x' => array(4, 5, 6)));

$result = $manager->executeQuery(NS, new MongoDB\Driver\Query(array('x' => 1)));
$result->setTypeMap(array("array" => "MyArrayObject"));

$documents = $result->toArray();

var_dump($documents[0]['x'] instanceof MyArrayObject);

?>
===DONE===
<?php exit(0); ?>
--EXPECT--
bool(true)
===DONE===
