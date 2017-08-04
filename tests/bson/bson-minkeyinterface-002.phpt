--TEST--
MongoDB\BSON\MinKeyInterface example with MongoDB\BSON\TypeWrapper
--FILE--
<?php

require_once __DIR__ . '/../utils/tools.php';

class MyMinKey implements MongoDB\BSON\TypeWrapper, MongoDB\BSON\MinKeyInterface
{
    private $minkey;

    public function __construct()
    {
        $this->minkey = new MongoDB\BSON\MinKey;
    }

    public static function createFromBSONType(MongoDB\BSON\Type $type)
    {
        if ( ! $type instanceof MongoDB\BSON\MinKey) {
            throw new InvalidArgumentException('Cannot create MyMinKey from ' . get_class($type));
        }

        return new self;
    }

    public function toBSONType()
    {
        return $this->minkey;
    }
}

$minkey = new MongoDB\BSON\MinKey;
$myMinkey = new MyMinKey;

echo "\nTesting MyMinKey and MinKey produce the same BSON\n";
var_dump(fromPHP(['x' => $minkey]) === fromPHP(['x' => $myMinkey]));

echo "\nTesting MinKey to BSON to MyMinKey\n";
var_dump(toPHP(fromPHP(['x' => $minkey]), ['types' => ['MinKey' => 'MyMinKey']])->x);

echo "\nTesting MyMinKey to BSON to MyMinKey\n";
var_dump(toPHP(fromPHP(['x' => $myMinkey]), ['types' => ['MinKey' => 'MyMinKey']])->x);

echo "\nTesting MyMinKey to BSON to MinKey\n";
var_dump(toPHP(fromPHP(['x' => $myMinkey]))->x);

echo "\nTesting MyMinKey::createFromBSONType() expects MinKey\n";
echo throws(function() {
    toPHP(fromPHP(['x' => new MongoDB\BSON\MaxKey]), ['types' => ['MaxKey' => 'MyMinKey']]);
}, 'InvalidArgumentException'), "\n";

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
Testing MyMinKey and MinKey produce the same BSON
bool(true)

Testing MinKey to BSON to MyMinKey
object(MyMinKey)#%d (%d) {
  ["minkey":"MyMinKey":private]=>
  object(MongoDB\BSON\MinKey)#%d (%d) {
  }
}

Testing MyMinKey to BSON to MyMinKey
object(MyMinKey)#%d (%d) {
  ["minkey":"MyMinKey":private]=>
  object(MongoDB\BSON\MinKey)#%d (%d) {
  }
}

Testing MyMinKey to BSON to MinKey
object(MongoDB\BSON\MinKey)#%d (%d) {
}

Testing MyMinKey::createFromBSONType() expects MinKey
OK: Got InvalidArgumentException
Cannot create MyMinKey from MongoDB\BSON\MaxKey
===DONE===
