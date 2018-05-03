--TEST--
MongoDB\Driver\Cursor::setTypeMap(): Setting using type "object"
--SKIPIF--
<?php require __DIR__ . "/../utils/basic-skipif.inc"; ?>
<?php skip_if_not_live(); ?>
<?php skip_if_not_clean(); ?>
--FILE--
<?php

require_once __DIR__ . "/../utils/basic.inc";

$manager = new MongoDB\Driver\Manager(URI);

$bulk = new MongoDB\Driver\BulkWrite();
$bulk->insert(array('_id' => 1, 'bson_array' => array(1, 2, 3), 'bson_object' => array("string" => "keys", "for" => "ever")));
$bulk->insert(array('_id' => 2, 'bson_array' => array(4, 5, 6)));
$manager->executeBulkWrite(NS, $bulk);

function fetch($manager, $typemap = array()) {
    $cursor = $manager->executeQuery(NS, new MongoDB\Driver\Query(array('bson_array' => 1)));
    if ($typemap) {
        $cursor->setTypeMap($typemap);
    }

    $documents = $cursor->toArray();
    return $documents;
}


echo "Setting to 'object' for arrays and 'array' for embedded and root documents\n";
$documents = fetch($manager, array("array" => "object", "document" => "array", "root" => "array"));
var_dump(is_array($documents[0]));
var_dump($documents[0]['bson_array'] instanceof stdClass);
var_dump(is_array($documents[0]['bson_object']));


echo "\nSetting to 'array' for arrays and 'object' for embedded and root documents\n";
$documents = fetch($manager, array("array" => "array", "document" => "object", "root" => "object"));
var_dump($documents[0] instanceof stdClass);
var_dump(is_array($documents[0]->bson_array));
var_dump($documents[0]->bson_object instanceof stdClass);


echo "\nSetting to 'object' for arrays, embedded, and root documents\n";
$documents = fetch($manager, array("array" => "object", "document" => "object", "root" => "object"));
var_dump($documents[0] instanceof stdClass);
var_dump($documents[0]->bson_array instanceof stdClass);
var_dump($documents[0]->bson_object instanceof stdClass);
?>
===DONE===
<?php exit(0); ?>
--EXPECT--
Setting to 'object' for arrays and 'array' for embedded and root documents
bool(true)
bool(true)
bool(true)

Setting to 'array' for arrays and 'object' for embedded and root documents
bool(true)
bool(true)
bool(true)

Setting to 'object' for arrays, embedded, and root documents
bool(true)
bool(true)
bool(true)
===DONE===
