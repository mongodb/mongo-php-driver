--TEST--
MongoDB\BSON\fromPHP(): Serializable with circular references
--FILE--
<?php

require_once __DIR__ . '/../utils/tools.php';

class MyRecursiveWrapper implements MongoDB\BSON\TypeWrapper
{
    private $bsonType;

    public function __construct($bsonType)
    {
        $this->bsonType = $bsonType;
    }

    public static function createFromBSONType(MongoDB\BSON\Type $bsonType)
    {
        return new self($bsonType);
    }

    public function toBSONType()
    {
        return $this;
    }
}

class MyIndirectlyRecursiveWrapper extends MyRecursiveWrapper
{
    public function toBSONType()
    {
        return ['x' => $this];
    }
}

echo "\nTesting TypeWrapper with direct circular reference\n";

echo throws(function() {
    fromPHP(['x' => new MyRecursiveWrapper(123)]);
}, 'MongoDB\Driver\Exception\UnexpectedValueException'), "\n";

echo "\nTesting TypeWrapper with indirect circular reference\n";

echo throws(function() {
    fromPHP(['x' => new MyIndirectlyRecursiveWrapper(123)]);
}, 'MongoDB\Driver\Exception\UnexpectedValueException'), "\n";

?>
===DONE===
<?php exit(0); ?>
--EXPECT--
Testing TypeWrapper with direct circular reference
OK: Got MongoDB\Driver\Exception\UnexpectedValueException
Detected recursion for fieldname "x"

Testing TypeWrapper with indirect circular reference
OK: Got MongoDB\Driver\Exception\UnexpectedValueException
Detected recursion for fieldname "x"
===DONE===
