--TEST--
MongoDB\BSON\Decimal128Interface example with MongoDB\BSON\TypeWrapper
--FILE--
<?php

require_once __DIR__ . '/../utils/tools.php';

class MyDecimal128 implements MongoDB\BSON\TypeWrapper, MongoDB\BSON\Decimal128Interface
{
    private $decimal;

    public function __construct($value)
    {
        $this->decimal = new MongoDB\BSON\Decimal128($value);
    }

    public static function createFromBSONType(MongoDB\BSON\Type $type)
    {
        if ( ! $type instanceof MongoDB\BSON\Decimal128) {
            throw new InvalidArgumentException('Cannot create MyDecimal128 from ' . get_class($type));
        }

        return new self($type);
    }

    public function toBSONType()
    {
        return $this->decimal;
    }

    public function __toString()
    {
        return (string) $this->decimal;
    }
}

$decimal = new MongoDB\BSON\Decimal128('1234.5678');
$myDecimal = new MyDecimal128('1234.5678');

echo "\nTesting MyDecimal128 and Decimal128 are equivalent\n";
var_dump((string) $decimal === (string) $myDecimal);

echo "\nTesting MyDecimal128 and Decimal128 produce the same BSON\n";
var_dump(fromPHP(['x' => $decimal]) === fromPHP(['x' => $myDecimal]));

echo "\nTesting Decimal128 to BSON to MyDecimal128\n";
var_dump(toPHP(fromPHP(['x' => $decimal]), ['types' => ['Decimal128' => 'MyDecimal128']])->x);

echo "\nTesting MyDecimal128 to BSON to MyDecimal128\n";
var_dump(toPHP(fromPHP(['x' => $myDecimal]), ['types' => ['Decimal128' => 'MyDecimal128']])->x);

echo "\nTesting MyDecimal128 to BSON to Decimal128\n";
var_dump(toPHP(fromPHP(['x' => $myDecimal]))->x);

echo "\nTesting MyDecimal128::createFromBSONType() expects Decimal128\n";
echo throws(function() {
    toPHP(fromPHP(['x' => new MongoDB\BSON\MinKey]), ['types' => ['MinKey' => 'MyDecimal128']]);
}, 'InvalidArgumentException'), "\n";

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
Testing MyDecimal128 and Decimal128 are equivalent
bool(true)

Testing MyDecimal128 and Decimal128 produce the same BSON
bool(true)

Testing Decimal128 to BSON to MyDecimal128
object(MyDecimal128)#%d (%d) {
  ["decimal":"MyDecimal128":private]=>
  object(MongoDB\BSON\Decimal128)#%d (%d) {
    ["dec"]=>
    string(9) "1234.5678"
  }
}

Testing MyDecimal128 to BSON to MyDecimal128
object(MyDecimal128)#%d (%d) {
  ["decimal":"MyDecimal128":private]=>
  object(MongoDB\BSON\Decimal128)#%d (%d) {
    ["dec"]=>
    string(9) "1234.5678"
  }
}

Testing MyDecimal128 to BSON to Decimal128
object(MongoDB\BSON\Decimal128)#%d (%d) {
  ["dec"]=>
  string(9) "1234.5678"
}

Testing MyDecimal128::createFromBSONType() expects Decimal128
OK: Got InvalidArgumentException
Cannot create MyDecimal128 from MongoDB\BSON\MinKey
===DONE===
