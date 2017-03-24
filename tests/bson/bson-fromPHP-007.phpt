--TEST--
MongoDB\BSON\fromPHP(): Type wrappers
--FILE--
<?php

require_once __DIR__ . '/../utils/tools.php';

class MyWrapper implements MongoDB\BSON\TypeWrapper
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
        return $this->bsonType;
    }
}

class MyDocument implements MongoDB\BSON\Serializable
{
    private $data;

    public function __construct($data = null)
    {
        $this->data = $data;
    }

    public function bsonSerialize()
    {
        return $this->data;
    }
}

$document = [
    // Basic PHP types
    'null' => new MyWrapper(null),
    'boolean' => new MyWrapper(true),
    'integer' => new MyWrapper(123),
    'double' => new MyWrapper(4.125),
    'string' => new MyWrapper('foo'),
    'array' => new MyWrapper([1, 2, 3]),
    'document' => new MyWrapper(['foo' => 1]),
    // MongoDB\BSON\Type classes
    'Binary' => new MyWrapper(new MongoDB\BSON\Binary('foo', MongoDB\BSON\Binary::TYPE_GENERIC)),
    'Decimal128' => new MyWrapper(new MongoDB\BSON\Decimal128('1234.5678')),
    'Javascript' => new MyWrapper(new MongoDB\BSON\Javascript('function foo() { return 1; }')),
    'MaxKey' => new MyWrapper(new MongoDB\BSON\MaxKey),
    'MinKey' => new MyWrapper(new MongoDB\BSON\MinKey),
    'ObjectID' => new MyWrapper(new MongoDB\BSON\ObjectId('58d17cef784c6b405fec16a1')),
    'Regex' => new MyWrapper(new MongoDB\BSON\Regex('pattern', 'i')),
    'Timestamp' => new MyWrapper(new MongoDB\BSON\Timestamp(1234, 5678)),
    'UTCDateTime' => new MyWrapper(new MongoDB\BSON\UTCDateTime(1490124067000)),
    // MongoDB\BSON\Serializable interface
    'Serializable' => new MyWrapper(new MyDocument(['bar' => 1])),
];

var_dump(toPHP(fromPHP($document)));

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
object(stdClass)#%d (%d) {
  ["null"]=>
  NULL
  ["boolean"]=>
  bool(true)
  ["integer"]=>
  int(123)
  ["double"]=>
  float(4.125)
  ["string"]=>
  string(3) "foo"
  ["array"]=>
  array(3) {
    [0]=>
    int(1)
    [1]=>
    int(2)
    [2]=>
    int(3)
  }
  ["document"]=>
  object(stdClass)#%d (%d) {
    ["foo"]=>
    int(1)
  }
  ["Binary"]=>
  object(MongoDB\BSON\Binary)#%d (%d) {
    ["data"]=>
    string(3) "foo"
    ["type"]=>
    int(0)
  }
  ["Decimal128"]=>
  object(MongoDB\BSON\Decimal128)#%d (%d) {
    ["dec"]=>
    string(9) "1234.5678"
  }
  ["Javascript"]=>
  object(MongoDB\BSON\Javascript)#%d (%d) {
    ["code"]=>
    string(28) "function foo() { return 1; }"
    ["scope"]=>
    NULL
  }
  ["MaxKey"]=>
  object(MongoDB\BSON\MaxKey)#%d (%d) {
  }
  ["MinKey"]=>
  object(MongoDB\BSON\MinKey)#%d (%d) {
  }
  ["ObjectID"]=>
  object(MongoDB\BSON\ObjectID)#%d (%d) {
    ["oid"]=>
    string(24) "58d17cef784c6b405fec16a1"
  }
  ["Regex"]=>
  object(MongoDB\BSON\Regex)#%d (%d) {
    ["pattern"]=>
    string(7) "pattern"
    ["flags"]=>
    string(1) "i"
  }
  ["Timestamp"]=>
  object(MongoDB\BSON\Timestamp)#%d (%d) {
    ["increment"]=>
    string(4) "1234"
    ["timestamp"]=>
    string(4) "5678"
  }
  ["UTCDateTime"]=>
  object(MongoDB\BSON\UTCDateTime)#%d (%d) {
    ["milliseconds"]=>
    string(13) "1490124067000"
  }
  ["Serializable"]=>
  object(stdClass)#%d (%d) {
    ["bar"]=>
    int(1)
  }
}
===DONE===
