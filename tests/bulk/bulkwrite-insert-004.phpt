--TEST--
MongoDB\Driver\BulkWrite::insert() returns "_id" of inserted document
--SKIPIF--
<?php require __DIR__ . "/../utils/basic-skipif.inc"; ?>
<?php NEEDS('STANDALONE'); CLEANUP(STANDALONE); ?>
--FILE--
<?php
require_once __DIR__ . "/../utils/basic.inc";

class MySerializableId implements MongoDB\BSON\Serializable
{
    public $id;

    public function __construct($id)
    {
        $this->id = $id;
    }

    public function bsonSerialize()
    {
        return ['id' => $this->id];
    }
}

class MyPersistableId extends MySerializableId implements MongoDB\BSON\Persistable
{
    public function bsonUnserialize(array $data)
    {
        $this->id = $data['id'];
    }
}

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

$documents = [
    ['x' => 1],
    ['_id' => new MongoDB\BSON\ObjectID('590b72d606e9660190656a55')],
    ['_id' => ['foo' => 1]],
    ['_id' => new MySerializableId('foo')],
    ['_id' => new MyPersistableId('bar')],
    ['_id' => new MyWrapper(new MongoDB\BSON\Binary('foo', 0))],
];

$manager = new MongoDB\Driver\Manager(STANDALONE);

$bulk = new MongoDB\Driver\BulkWrite();

foreach ($documents as $document) {
    var_dump($bulk->insert($document));
}

$result = $manager->executeBulkWrite(NS, $bulk);
printf("Inserted %d document(s)\n", $result->getInsertedCount());

$cursor = $manager->executeQuery(NS, new MongoDB\Driver\Query([]));
var_dump($cursor->toArray());

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
object(MongoDB\BSON\ObjectID)#%d (%d) {
  ["oid"]=>
  string(24) "%x"
}
object(MongoDB\BSON\ObjectID)#%d (%d) {
  ["oid"]=>
  string(24) "590b72d606e9660190656a55"
}
object(stdClass)#%d (%d) {
  ["foo"]=>
  int(1)
}
object(stdClass)#%d (%d) {
  ["id"]=>
  string(3) "foo"
}
object(MyPersistableId)#%d (%d) {
  ["id"]=>
  string(3) "bar"
}
object(MongoDB\BSON\Binary)#%d (%d) {
  ["data"]=>
  string(3) "foo"
  ["type"]=>
  int(0)
}
Inserted 6 document(s)
array(6) {
  [0]=>
  object(stdClass)#%d (%d) {
    ["_id"]=>
    object(MongoDB\BSON\ObjectID)#%d (%d) {
      ["oid"]=>
      string(24) "%x"
    }
    ["x"]=>
    int(1)
  }
  [1]=>
  object(stdClass)#%d (%d) {
    ["_id"]=>
    object(MongoDB\BSON\ObjectID)#%d (%d) {
      ["oid"]=>
      string(24) "590b72d606e9660190656a55"
    }
  }
  [2]=>
  object(stdClass)#%d (%d) {
    ["_id"]=>
    object(stdClass)#%d (%d) {
      ["foo"]=>
      int(1)
    }
  }
  [3]=>
  object(stdClass)#%d (%d) {
    ["_id"]=>
    object(stdClass)#%d (%d) {
      ["id"]=>
      string(3) "foo"
    }
  }
  [4]=>
  object(stdClass)#%d (%d) {
    ["_id"]=>
    object(MyPersistableId)#%d (%d) {
      ["id"]=>
      string(3) "bar"
    }
  }
  [5]=>
  object(stdClass)#%d (%d) {
    ["_id"]=>
    object(MongoDB\BSON\Binary)#%d (%d) {
      ["data"]=>
      string(3) "foo"
      ["type"]=>
      int(0)
    }
  }
}
===DONE===
