--TEST--
MongoDB\BSON\MaxKeyInterface example with MongoDB\BSON\TypeWrapper
--FILE--
<?php

require_once __DIR__ . '/../utils/tools.php';

class MyMaxKey implements MongoDB\BSON\TypeWrapper, MongoDB\BSON\MaxKeyInterface
{
    private $maxkey;

    public function __construct()
    {
        $this->maxkey = new MongoDB\BSON\MaxKey;
    }

    public static function createFromBSONType(MongoDB\BSON\Type $type)
    {
        if ( ! $type instanceof MongoDB\BSON\MaxKey) {
            throw new InvalidArgumentException('Cannot create MyMaxKey from ' . get_class($type));
        }

        return new self;
    }

    public function toBSONType()
    {
        return $this->maxkey;
    }
}

$maxkey = new MongoDB\BSON\MaxKey;
$myMaxkey = new MyMaxKey;

echo "\nTesting MyMaxKey and MaxKey produce the same BSON\n";
var_dump(fromPHP(['x' => $maxkey]) === fromPHP(['x' => $myMaxkey]));

echo "\nTesting MaxKey to BSON to MyMaxKey\n";
var_dump(toPHP(fromPHP(['x' => $maxkey]), ['types' => ['MaxKey' => 'MyMaxKey']])->x);

echo "\nTesting MyMaxKey to BSON to MyMaxKey\n";
var_dump(toPHP(fromPHP(['x' => $myMaxkey]), ['types' => ['MaxKey' => 'MyMaxKey']])->x);

echo "\nTesting MyMaxKey to BSON to MaxKey\n";
var_dump(toPHP(fromPHP(['x' => $myMaxkey]))->x);

echo "\nTesting MyMaxKey::createFromBSONType() expects MaxKey\n";
echo throws(function() {
    toPHP(fromPHP(['x' => new MongoDB\BSON\MinKey]), ['types' => ['MinKey' => 'MyMaxKey']]);
}, 'InvalidArgumentException'), "\n";

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
Testing MyMaxKey and MaxKey produce the same BSON
bool(true)

Testing MaxKey to BSON to MyMaxKey
object(MyMaxKey)#%d (%d) {
  ["maxkey":"MyMaxKey":private]=>
  object(MongoDB\BSON\MaxKey)#%d (%d) {
  }
}

Testing MyMaxKey to BSON to MyMaxKey
object(MyMaxKey)#%d (%d) {
  ["maxkey":"MyMaxKey":private]=>
  object(MongoDB\BSON\MaxKey)#%d (%d) {
  }
}

Testing MyMaxKey to BSON to MaxKey
object(MongoDB\BSON\MaxKey)#%d (%d) {
}

Testing MyMaxKey::createFromBSONType() expects MaxKey
OK: Got InvalidArgumentException
Cannot create MyMaxKey from MongoDB\BSON\MinKey
===DONE===
