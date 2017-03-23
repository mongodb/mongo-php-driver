--TEST--
MongoDB\BSON\toPHP(): Type wrappers
--FILE--
<?php

require_once __DIR__ . '/../utils/tools.php';

class MyWrapper implements MongoDB\BSON\TypeWrapper
{
    private $bsonType;

    public function __construct(MongoDB\BSON\Type $bsonType)
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

$types = [
    'Binary',
    'Decimal128',
    'Javascript',
    'MaxKey',
    'MinKey',
    'ObjectID',
    'Regex',
    'Timestamp',
    'UTCDateTime',
];

$document = [
    'Binary' => new MongoDB\BSON\Binary('foo', MongoDB\BSON\Binary::TYPE_GENERIC),
    'Decimal128' => new MongoDB\BSON\Decimal128('1234.5678'),
    'Javascript' => new MongoDB\BSON\Javascript('function foo() { return 1; }'),
    'MaxKey' => new MongoDB\BSON\MaxKey,
    'MinKey' => new MongoDB\BSON\MinKey,
    'ObjectID' => new MongoDB\BSON\ObjectId('58d17cef784c6b405fec16a1'),
    'Regex' => new MongoDB\BSON\Regex('pattern', 'i'),
    'Timestamp' => new MongoDB\BSON\Timestamp(1234, 5678),
    'UTCDateTime' => new MongoDB\BSON\UTCDateTime(1490124067000),
];

foreach ($types as $type) {
    $typeMap['types'][$type] = 'MyWrapper';
}

var_dump(toPHP(fromPHP($document), $typeMap));

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
object(stdClass)#%d (%d) {
  ["Binary"]=>
  object(MyWrapper)#%d (%d) {
    ["bsonType":"MyWrapper":private]=>
    object(MongoDB\BSON\Binary)#%d (%d) {
      ["data"]=>
      string(3) "foo"
      ["type"]=>
      int(0)
    }
  }
  ["Decimal128"]=>
  object(MyWrapper)#%d (%d) {
    ["bsonType":"MyWrapper":private]=>
    object(MongoDB\BSON\Decimal128)#%d (%d) {
      ["dec"]=>
      string(9) "1234.5678"
    }
  }
  ["Javascript"]=>
  object(MyWrapper)#%d (%d) {
    ["bsonType":"MyWrapper":private]=>
    object(MongoDB\BSON\Javascript)#%d (%d) {
      ["code"]=>
      string(28) "function foo() { return 1; }"
      ["scope"]=>
      NULL
    }
  }
  ["MaxKey"]=>
  object(MyWrapper)#%d (%d) {
    ["bsonType":"MyWrapper":private]=>
    object(MongoDB\BSON\MaxKey)#%d (%d) {
    }
  }
  ["MinKey"]=>
  object(MyWrapper)#%d (%d) {
    ["bsonType":"MyWrapper":private]=>
    object(MongoDB\BSON\MinKey)#%d (%d) {
    }
  }
  ["ObjectID"]=>
  object(MyWrapper)#%d (%d) {
    ["bsonType":"MyWrapper":private]=>
    object(MongoDB\BSON\ObjectID)#%d (%d) {
      ["oid"]=>
      string(24) "58d17cef784c6b405fec16a1"
    }
  }
  ["Regex"]=>
  object(MyWrapper)#%d (%d) {
    ["bsonType":"MyWrapper":private]=>
    object(MongoDB\BSON\Regex)#%d (%d) {
      ["pattern"]=>
      string(7) "pattern"
      ["flags"]=>
      string(1) "i"
    }
  }
  ["Timestamp"]=>
  object(MyWrapper)#%d (%d) {
    ["bsonType":"MyWrapper":private]=>
    object(MongoDB\BSON\Timestamp)#%d (%d) {
      ["increment"]=>
      string(4) "1234"
      ["timestamp"]=>
      string(4) "5678"
    }
  }
  ["UTCDateTime"]=>
  object(MyWrapper)#%d (%d) {
    ["bsonType":"MyWrapper":private]=>
    object(MongoDB\BSON\UTCDateTime)#%d (%d) {
      ["milliseconds"]=>
      string(13) "1490124067000"
    }
  }
}
===DONE===
