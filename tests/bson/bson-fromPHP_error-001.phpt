--TEST--
BSON\fromPHP(): bsonSerialize() must return an array or stdClass
--SKIPIF--
<?php require __DIR__ . "/../utils/basic-skipif.inc"?>
--FILE--
<?php
use MongoDB\BSON as BSON;

require_once __DIR__ . "/../utils/basic.inc";

class MyDocument implements BSON\Serializable
{
    private $data;

    public function __construct($data = null)
    {
        $this->data = $data;
    }

    public function bsonSerialize()
    {
        return $this->data;
    }
}

$invalidValues = array(null, 123, 'foo', true, new MyDocument);

echo "Testing top-level objects\n";

foreach ($invalidValues as $invalidValue) {
    try {
        hex_dump(fromPHP(new MyDocument($invalidValue)));
    } catch (MongoDB\Driver\Exception\UnexpectedValueException $e) {
        echo $e->getMessage(), "\n";
    }
}

echo "\nTesting nested objects\n";

foreach ($invalidValues as $invalidValue) {
    try {
        hex_dump(fromPHP(new MyDocument(array('nested' => new MyDocument($invalidValue)))));
    } catch (MongoDB\Driver\Exception\UnexpectedValueException $e) {
        echo $e->getMessage(), "\n";
    }
}

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
Testing top-level objects
Expected MyDocument::bsonSerialize() to return an array or stdClass, %r(null|NULL)%r given
Expected MyDocument::bsonSerialize() to return an array or stdClass, integer given
Expected MyDocument::bsonSerialize() to return an array or stdClass, string given
Expected MyDocument::bsonSerialize() to return an array or stdClass, boolean given
Expected MyDocument::bsonSerialize() to return an array or stdClass, MyDocument given

Testing nested objects
Expected MyDocument::bsonSerialize() to return an array or stdClass, %r(null|NULL)%r given
Expected MyDocument::bsonSerialize() to return an array or stdClass, integer given
Expected MyDocument::bsonSerialize() to return an array or stdClass, string given
Expected MyDocument::bsonSerialize() to return an array or stdClass, boolean given
Expected MyDocument::bsonSerialize() to return an array or stdClass, MyDocument given
===DONE===
