--TEST--
MongoDB\BSON\fromPHP(): Serializable with circular references
--FILE--
<?php

require_once __DIR__ . '/../utils/basic.inc';

class MyRecursiveSerializable implements MongoDB\BSON\Serializable
{
    public $child = 1;

    #[\ReturnTypeWillChange]
    public function bsonSerialize()
    {
        return $this;
    }
}

class MyIndirectlyRecursiveSerializable extends MyRecursiveSerializable
{
    #[\ReturnTypeWillChange]
    public function bsonSerialize()
    {
        return ['parent' => $this];
    }
}

echo "\nTesting Serializable with direct circular reference\n";

echo throws(function() {
    MongoDB\BSON\fromPHP(new MyRecursiveSerializable);
}, 'MongoDB\Driver\Exception\UnexpectedValueException'), "\n";

echo "\nTesting Serializable with indirect circular reference\n";

echo throws(function() {
    MongoDB\BSON\fromPHP(new MyIndirectlyRecursiveSerializable);
}, 'MongoDB\Driver\Exception\UnexpectedValueException'), "\n";

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
Testing Serializable with direct circular reference

Deprecated: Function MongoDB\BSON\fromPHP() is deprecated in %s
OK: Got MongoDB\Driver\Exception\UnexpectedValueException
Expected MyRecursiveSerializable::bsonSerialize() to return an array, stdClass, MongoDB\BSON\Document, or MongoDB\BSON\PackedArray, MyRecursiveSerializable given

Testing Serializable with indirect circular reference

Deprecated: Function MongoDB\BSON\fromPHP() is deprecated in %s
OK: Got MongoDB\Driver\Exception\UnexpectedValueException
Detected recursion for field path "parent.parent"
===DONE===
