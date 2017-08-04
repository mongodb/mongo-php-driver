--TEST--
MongoDB\BSON\BinaryInterface example with MongoDB\BSON\TypeWrapper
--FILE--
<?php

require_once __DIR__ . '/../utils/tools.php';

class MyBinary implements MongoDB\BSON\TypeWrapper, MongoDB\BSON\BinaryInterface
{
    private $binary;

    public function __construct($data, $type)
    {
        $this->binary = new MongoDB\BSON\Binary($data, $type);
    }

    public static function createFromBSONType(MongoDB\BSON\Type $type)
    {
        if ( ! $type instanceof MongoDB\BSON\Binary) {
            throw new InvalidArgumentException('Cannot create MyBinary from ' . get_class($type));
        }

        return new self($type->getData(), $type->getType());
    }

    public function toBSONType()
    {
        return $this->binary;
    }

    public function getData()
    {
        return $this->binary->getData();
    }

    public function getType()
    {
        return $this->binary->getType();
    }

    public function __toString()
    {
        return (string) $this->binary;
    }
}

$binary = new MongoDB\BSON\Binary('foo', MongoDB\BSON\Binary::TYPE_GENERIC);
$myBinary = new MyBinary('foo', MongoDB\BSON\Binary::TYPE_GENERIC);

echo "\nTesting MyBinary and Binary are equivalent\n";
var_dump($binary->getData() === $myBinary->getData());
var_dump($binary->getType() === $myBinary->getType());
var_dump((string) $binary === (string) $myBinary);

echo "\nTesting MyBinary and Binary produce the same BSON\n";
var_dump(fromPHP(['x' => $binary]) === fromPHP(['x' => $myBinary]));

echo "\nTesting Binary to BSON to MyBinary\n";
var_dump(toPHP(fromPHP(['x' => $binary]), ['types' => ['Binary' => 'MyBinary']])->x);

echo "\nTesting MyBinary to BSON to MyBinary\n";
var_dump(toPHP(fromPHP(['x' => $myBinary]), ['types' => ['Binary' => 'MyBinary']])->x);

echo "\nTesting MyBinary to BSON to Binary\n";
var_dump(toPHP(fromPHP(['x' => $myBinary]))->x);

echo "\nTesting MyBinary::createFromBSONType() expects Binary\n";
echo throws(function() {
    toPHP(fromPHP(['x' => new MongoDB\BSON\MinKey]), ['types' => ['MinKey' => 'MyBinary']]);
}, 'InvalidArgumentException'), "\n";

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
Testing MyBinary and Binary are equivalent
bool(true)
bool(true)
bool(true)

Testing MyBinary and Binary produce the same BSON
bool(true)

Testing Binary to BSON to MyBinary
object(MyBinary)#%d (%d) {
  ["binary":"MyBinary":private]=>
  object(MongoDB\BSON\Binary)#%d (%d) {
    ["data"]=>
    string(3) "foo"
    ["type"]=>
    int(0)
  }
}

Testing MyBinary to BSON to MyBinary
object(MyBinary)#%d (%d) {
  ["binary":"MyBinary":private]=>
  object(MongoDB\BSON\Binary)#%d (%d) {
    ["data"]=>
    string(3) "foo"
    ["type"]=>
    int(0)
  }
}

Testing MyBinary to BSON to Binary
object(MongoDB\BSON\Binary)#%d (%d) {
  ["data"]=>
  string(3) "foo"
  ["type"]=>
  int(0)
}

Testing MyBinary::createFromBSONType() expects Binary
OK: Got InvalidArgumentException
Cannot create MyBinary from MongoDB\BSON\MinKey
===DONE===
