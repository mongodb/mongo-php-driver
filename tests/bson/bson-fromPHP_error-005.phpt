--TEST--
MongoDB\BSON\fromPHP(): Serializable with circular references
--FILE--
<?php

require_once __DIR__ . '/../utils/tools.php';

class MyRecursiveSerializable implements MongoDB\BSON\Serializable
{
    public $child = 1;

    public function bsonSerialize()
    {
        return $this;
    }
}

class MyIndirectlyRecursiveSerializable extends MyRecursiveSerializable
{
    public function bsonSerialize()
    {
        return ['parent' => $this];
    }
}

echo "\nTesting Serializable with direct circular reference\n";

echo throws(function() {
    fromPHP(new MyRecursiveSerializable);
}, 'MongoDB\Driver\Exception\UnexpectedValueException'), "\n";

echo "\nTesting Serializable with indirect circular reference\n";

echo throws(function() {
    fromPHP(new MyIndirectlyRecursiveSerializable);
}, 'MongoDB\Driver\Exception\UnexpectedValueException'), "\n";

?>
===DONE===
<?php exit(0); ?>
--EXPECT--
Testing Serializable with direct circular reference
OK: Got MongoDB\Driver\Exception\UnexpectedValueException
Expected MyRecursiveSerializable::bsonSerialize() to return an array or stdClass, MyRecursiveSerializable given

Testing Serializable with indirect circular reference
OK: Got MongoDB\Driver\Exception\UnexpectedValueException
Detected recursion for field path "parent.parent"
===DONE===
