--TEST--
MongoDB\Driver\Cursor::setTypeMap(): Setting typemaps
--SKIPIF--
<?php require __DIR__ . "/../utils/basic-skipif.inc"; CLEANUP(STANDALONE) ?>
--FILE--
<?php
use MongoDB\BSON as BSON;

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


/* Default */
$documents = fetch($manager);
var_dump(is_array($documents[0]['bson_array']));
var_dump(is_object($documents[0]['bson_object']));


/* Setting to MyArrayObject */
$documents = fetch($manager, array("array" => "MyArrayObject"));
var_dump($documents[0]['bson_array'] instanceof MyArrayObject);
var_dump(is_object($documents[0]['bson_object']));

/* Setting to MyArrayObject & MyArrayObject */
$documents = fetch($manager, array("array" => "MyArrayObject", "document" => "MyArrayObject"));
var_dump($documents[0]['bson_array'] instanceof MyArrayObject);
var_dump($documents[0]['bson_object'] instanceof MyArrayObject);


/* Setting to array */
$documents = fetch($manager, array("array" => "array", "document" => "array"));
var_dump(is_array($documents[0]['bson_array']));
var_dump(is_array($documents[0]['bson_object']));


/* Setting to stdlcass & array */
$documents = fetch($manager, array("array" => "stdclass", "document" => "array"));
var_dump(is_object($documents[0]['bson_array']));
var_dump(is_array($documents[0]['bson_object']));


/* Setting to array & stdclass */
$documents = fetch($manager, array("array" => "array", "document" => "stdclass"));
var_dump(is_array($documents[0]['bson_array']));
var_dump(is_object($documents[0]['bson_object']));


/* Setting to stdclass */
$documents = fetch($manager, array("array" => "stdclass", "document" => "stdclass"));
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
bool(true)
bool(true)
bool(true)
bool(true)
bool(true)
bool(true)
bool(true)
bool(true)
===DONE===
