--TEST--
MongoDB\BSON\TimestampInterface example with MongoDB\BSON\TypeWrapper
--FILE--
<?php

require_once __DIR__ . '/../utils/tools.php';

class MyTimestamp implements MongoDB\BSON\TypeWrapper, MongoDB\BSON\TimestampInterface
{
    private $timestamp;

    public function __construct($increment, $timestamp)
    {
        $this->timestamp = new MongoDB\BSON\Timestamp($increment, $timestamp);
    }

    public static function createFromBSONType(MongoDB\BSON\Type $type)
    {
        if ( ! $type instanceof MongoDB\BSON\Timestamp) {
            throw new InvalidArgumentException('Cannot create MyTimestamp from ' . get_class($type));
        }

        return new self($type->getIncrement(), $type->getTimestamp());
    }

    public function toBSONType()
    {
        return $this->timestamp;
    }

    public function getIncrement()
    {
        return $this->timestamp->getIncrement();
    }

    public function getTimestamp()
    {
        return $this->timestamp->getTimestamp();
    }

    public function __toString()
    {
        return (string) $this->timestamp;
    }
}

$timestamp = new MongoDB\BSON\Timestamp(1234, 5678);
$myTimestamp = new MyTimestamp(1234, 5678);

echo "\nTesting MyTimestamp and Timestamp are equivalent\n";
var_dump($timestamp->getIncrement() === $myTimestamp->getIncrement());
var_dump($timestamp->getTimestamp() === $myTimestamp->getTimestamp());
var_dump((string) $timestamp === (string) $myTimestamp);

echo "\nTesting MyTimestamp and Timestamp produce the same BSON\n";
var_dump(fromPHP(['x' => $timestamp]) === fromPHP(['x' => $myTimestamp]));

echo "\nTesting Timestamp to BSON to MyTimestamp\n";
var_dump(toPHP(fromPHP(['x' => $timestamp]), ['types' => ['Timestamp' => 'MyTimestamp']])->x);

echo "\nTesting MyTimestamp to BSON to MyTimestamp\n";
var_dump(toPHP(fromPHP(['x' => $myTimestamp]), ['types' => ['Timestamp' => 'MyTimestamp']])->x);

echo "\nTesting MyTimestamp to BSON to Timestamp\n";
var_dump(toPHP(fromPHP(['x' => $myTimestamp]))->x);

echo "\nTesting MyTimestamp::createFromBSONType() expects Timestamp\n";
echo throws(function() {
    toPHP(fromPHP(['x' => new MongoDB\BSON\MinKey]), ['types' => ['MinKey' => 'MyTimestamp']]);
}, 'InvalidArgumentException'), "\n";

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
Testing MyTimestamp and Timestamp are equivalent
bool(true)
bool(true)
bool(true)

Testing MyTimestamp and Timestamp produce the same BSON
bool(true)

Testing Timestamp to BSON to MyTimestamp
object(MyTimestamp)#%d (%d) {
  ["timestamp":"MyTimestamp":private]=>
  object(MongoDB\BSON\Timestamp)#%d (%d) {
    ["increment"]=>
    string(4) "1234"
    ["timestamp"]=>
    string(4) "5678"
  }
}

Testing MyTimestamp to BSON to MyTimestamp
object(MyTimestamp)#%d (%d) {
  ["timestamp":"MyTimestamp":private]=>
  object(MongoDB\BSON\Timestamp)#%d (%d) {
    ["increment"]=>
    string(4) "1234"
    ["timestamp"]=>
    string(4) "5678"
  }
}

Testing MyTimestamp to BSON to Timestamp
object(MongoDB\BSON\Timestamp)#%d (%d) {
  ["increment"]=>
  string(4) "1234"
  ["timestamp"]=>
  string(4) "5678"
}

Testing MyTimestamp::createFromBSONType() expects Timestamp
OK: Got InvalidArgumentException
Cannot create MyTimestamp from MongoDB\BSON\MinKey
===DONE===
