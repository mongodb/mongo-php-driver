--TEST--
MongoDB\BSON\ObjectIDInterface example with MongoDB\BSON\TypeWrapper
--FILE--
<?php

require_once __DIR__ . '/../utils/tools.php';

class MyObjectID implements MongoDB\BSON\TypeWrapper, MongoDB\BSON\ObjectIDInterface
{
    private $oid;

    public function __construct($value = null)
    {
        $this->oid = new MongoDB\BSON\ObjectID($value);
    }

    public static function createFromBSONType(MongoDB\BSON\Type $type)
    {
        if ( ! $type instanceof MongoDB\BSON\ObjectID) {
            throw new InvalidArgumentException('Cannot create MyObjectID from ' . get_class($type));
        }

        return new self($type);
    }

    public function toBSONType()
    {
        return $this->oid;
    }

    public function getTimestamp()
    {
        return $this->oid->getTimestamp();
    }

    public function __toString()
    {
        return (string) $this->oid;
    }
}

$oid = new MongoDB\BSON\ObjectID('59823d5c98ed91f9d815743c');
$myOid = new MyObjectID('59823d5c98ed91f9d815743c');

echo "\nTesting MyObjectID and ObjectID are equivalent\n";
var_dump($oid->getTimestamp() === $myOid->getTimestamp());
var_dump((string) $oid === (string) $myOid);

echo "\nTesting MyObjectID and ObjectID produce the same BSON\n";
var_dump(fromPHP(['x' => $oid]) === fromPHP(['x' => $myOid]));

echo "\nTesting ObjectID to BSON to MyObjectID\n";
var_dump(toPHP(fromPHP(['x' => $oid]), ['types' => ['ObjectID' => 'MyObjectID']])->x);

echo "\nTesting MyObjectID to BSON to MyObjectID\n";
var_dump(toPHP(fromPHP(['x' => $myOid]), ['types' => ['ObjectID' => 'MyObjectID']])->x);

echo "\nTesting MyObjectID to BSON to ObjectID\n";
var_dump(toPHP(fromPHP(['x' => $myOid]))->x);

echo "\nTesting MyObjectID::createFromBSONType() expects ObjectID\n";
echo throws(function() {
    toPHP(fromPHP(['x' => new MongoDB\BSON\MinKey]), ['types' => ['MinKey' => 'MyObjectID']]);
}, 'InvalidArgumentException'), "\n";

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
Testing MyObjectID and ObjectID are equivalent
bool(true)
bool(true)

Testing MyObjectID and ObjectID produce the same BSON
bool(true)

Testing ObjectID to BSON to MyObjectID
object(MyObjectID)#%d (%d) {
  ["oid":"MyObjectID":private]=>
  object(MongoDB\BSON\ObjectID)#%d (%d) {
    ["oid"]=>
    string(24) "59823d5c98ed91f9d815743c"
  }
}

Testing MyObjectID to BSON to MyObjectID
object(MyObjectID)#%d (%d) {
  ["oid":"MyObjectID":private]=>
  object(MongoDB\BSON\ObjectID)#%d (%d) {
    ["oid"]=>
    string(24) "59823d5c98ed91f9d815743c"
  }
}

Testing MyObjectID to BSON to ObjectID
object(MongoDB\BSON\ObjectID)#%d (%d) {
  ["oid"]=>
  string(24) "59823d5c98ed91f9d815743c"
}

Testing MyObjectID::createFromBSONType() expects ObjectID
OK: Got InvalidArgumentException
Cannot create MyObjectID from MongoDB\BSON\MinKey
===DONE===
