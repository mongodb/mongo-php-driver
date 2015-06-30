--TEST--
BSON encoding error when bsonSerialize() for embedded document does not return an array
--SKIPIF--
<?php require __DIR__ . "/../utils/basic-skipif.inc"?>
--FILE--
<?php
use MongoDB\BSON as BSON;

require_once __DIR__ . "/../utils/basic.inc";

class MyClass implements BSON\Serializable
{
    private $value;

    public function __construct($value)
    {
        $this->value = $value;
    }

    public function bsonSerialize()
    {
        return $this->value;
    }
}

$invalidValues = array(new stdClass, 'foo', 1, true);

foreach ($invalidValues as $invalidValue) {
    try {
        $bson = fromArray(array('embed' => new MyClass($invalidValue)));
    } catch (MongoDB\Driver\Exception\UnexpectedValueException $e) {
        echo $e->getMessage(), "\n";
    }
}

?>
===DONE===
<?php exit(0); ?>
--EXPECT--
Expected bsonSerialize() to return an array, object given
Expected bsonSerialize() to return an array, string given
Expected bsonSerialize() to return an array, integer given
Expected bsonSerialize() to return an array, boolean given
===DONE===
