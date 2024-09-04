--TEST--
MongoDB\BSON\fromPHP(): bsonSerialize() must return an array or stdClass
--FILE--
<?php

require_once __DIR__ . '/../utils/basic.inc';

class MyDocument implements MongoDB\BSON\Serializable
{
    private $data;

    public function __construct($data = null)
    {
        $this->data = $data;
    }

    #[\ReturnTypeWillChange]
    public function bsonSerialize()
    {
        return $this->data;
    }
}

$invalidValues = array(null, 123, 'foo', true, new MyDocument);

echo "Testing top-level objects\n";

foreach ($invalidValues as $invalidValue) {
    try {
        hex_dump(MongoDB\BSON\fromPHP(new MyDocument($invalidValue)));
    } catch (MongoDB\Driver\Exception\UnexpectedValueException $e) {
        echo $e->getMessage(), "\n";
    }
}

echo "\nTesting nested objects\n";

foreach ($invalidValues as $invalidValue) {
    try {
        hex_dump(MongoDB\BSON\fromPHP(new MyDocument(array('nested' => new MyDocument($invalidValue)))));
    } catch (MongoDB\Driver\Exception\UnexpectedValueException $e) {
        echo $e->getMessage(), "\n";
    }
}

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
Testing top-level objects

Deprecated: Function MongoDB\BSON\fromPHP() is deprecated in %s
Expected MyDocument::bsonSerialize() to return an array, stdClass, MongoDB\BSON\Document, or MongoDB\BSON\PackedArray, null given

Deprecated: Function MongoDB\BSON\fromPHP() is deprecated in %s
Expected MyDocument::bsonSerialize() to return an array, stdClass, MongoDB\BSON\Document, or MongoDB\BSON\PackedArray, int given

Deprecated: Function MongoDB\BSON\fromPHP() is deprecated in %s
Expected MyDocument::bsonSerialize() to return an array, stdClass, MongoDB\BSON\Document, or MongoDB\BSON\PackedArray, string given

Deprecated: Function MongoDB\BSON\fromPHP() is deprecated in %s
Expected MyDocument::bsonSerialize() to return an array, stdClass, MongoDB\BSON\Document, or MongoDB\BSON\PackedArray, bool given

Deprecated: Function MongoDB\BSON\fromPHP() is deprecated in %s
Expected MyDocument::bsonSerialize() to return an array, stdClass, MongoDB\BSON\Document, or MongoDB\BSON\PackedArray, MyDocument given

Testing nested objects

Deprecated: Function MongoDB\BSON\fromPHP() is deprecated in %s
Expected MyDocument::bsonSerialize() to return an array, stdClass, MongoDB\BSON\Document, or MongoDB\BSON\PackedArray, null given

Deprecated: Function MongoDB\BSON\fromPHP() is deprecated in %s
Expected MyDocument::bsonSerialize() to return an array, stdClass, MongoDB\BSON\Document, or MongoDB\BSON\PackedArray, int given

Deprecated: Function MongoDB\BSON\fromPHP() is deprecated in %s
Expected MyDocument::bsonSerialize() to return an array, stdClass, MongoDB\BSON\Document, or MongoDB\BSON\PackedArray, string given

Deprecated: Function MongoDB\BSON\fromPHP() is deprecated in %s
Expected MyDocument::bsonSerialize() to return an array, stdClass, MongoDB\BSON\Document, or MongoDB\BSON\PackedArray, bool given

Deprecated: Function MongoDB\BSON\fromPHP() is deprecated in %s
Expected MyDocument::bsonSerialize() to return an array, stdClass, MongoDB\BSON\Document, or MongoDB\BSON\PackedArray, MyDocument given
===DONE===
