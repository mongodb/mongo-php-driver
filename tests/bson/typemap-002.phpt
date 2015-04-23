--TEST--
MongoDB\Driver\Cursor::setTypeMap(): Setting using type "object"
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

$manager->executeInsert(NS, array('_id' => 1, 'bson_array' => array(1, 2, 3), 'bson_object' => array("string" => "keys", "for" => "ever")));
$manager->executeInsert(NS, array('_id' => 2, 'bson_array' => array(4, 5, 6)));

function fetch($manager, $typemap = array()) {
    $cursor = $manager->executeQuery(NS, new MongoDB\Driver\Query(array('bson_array' => 1)));
    if ($typemap) {
        $cursor->setTypeMap($typemap);
    }

    $documents = $cursor->toArray();
    return $documents;
}



/* Setting to stdlcass & array */
$documents = fetch($manager, array("array" => "object", "document" => "array"));
var_dump(is_object($documents[0]['bson_array']));
var_dump(is_array($documents[0]['bson_object']));


/* Setting to array & object */
$documents = fetch($manager, array("array" => "array", "document" => "object"));
var_dump(is_array($documents[0]['bson_array']));
var_dump(is_object($documents[0]['bson_object']));


/* Setting to object */
$documents = fetch($manager, array("array" => "object", "document" => "object"));
var_dump(is_object($documents[0]['bson_array']));
var_dump(is_object($documents[0]['bson_object']));
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
===DONE===
