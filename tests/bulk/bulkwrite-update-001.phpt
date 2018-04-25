--TEST--
MongoDB\Driver\BulkWrite::update() should always encode __pclass for Persistable objects
--SKIPIF--
<?php require __DIR__ . "/../utils/basic-skipif.inc"; ?>
<?php skip_if_not_live(); ?>
<?php skip_if_not_clean(); ?>
--FILE--
<?php
require_once __DIR__ . "/../utils/basic.inc";

class MyClass implements MongoDB\BSON\Persistable
{
    private $id;
    private $child;

    public function __construct($id, MyClass $child = null)
    {
        $this->id = $id;
        $this->child = $child;
    }

    public function bsonSerialize()
    {
        return [
            '_id' => $this->id,
            'child' => $this->child,
        ];
    }

    public function bsonUnserialize(array $data)
    {
        $this->id = $data['_id'];
        $this->child = $data['child'];
    }
}

$manager = new MongoDB\Driver\Manager(URI);

$document = new MyClass('foo', new MyClass('bar', new MyClass('baz')));

$bulk = new MongoDB\Driver\BulkWrite();
$bulk->update(
    ['_id' => 'foo'],
    $document,
    ['upsert' => true]
);
$result = $manager->executeBulkWrite(NS, $bulk);
printf("Upserted %d document(s)\n", $result->getUpsertedCount());

$cursor = $manager->executeQuery(NS, new MongoDB\Driver\Query([]));
var_dump($cursor->toArray());

$bulk = new MongoDB\Driver\BulkWrite();
$bulk->update(
    $document,
    ['$set' => ['child' => new MyClass('yip', new MyClass('yap'))]]
);
$result = $manager->executeBulkWrite(NS, $bulk);
printf("Modified %d document(s)\n", $result->getModifiedCount());

$cursor = $manager->executeQuery(NS, new MongoDB\Driver\Query([]));
var_dump($cursor->toArray());

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
Upserted 1 document(s)
array(1) {
  [0]=>
  object(MyClass)#%d (%d) {
    ["id":"MyClass":private]=>
    string(3) "foo"
    ["child":"MyClass":private]=>
    object(MyClass)#%d (%d) {
      ["id":"MyClass":private]=>
      string(3) "bar"
      ["child":"MyClass":private]=>
      object(MyClass)#%d (%d) {
        ["id":"MyClass":private]=>
        string(3) "baz"
        ["child":"MyClass":private]=>
        NULL
      }
    }
  }
}
Modified 1 document(s)
array(1) {
  [0]=>
  object(MyClass)#%d (%d) {
    ["id":"MyClass":private]=>
    string(3) "foo"
    ["child":"MyClass":private]=>
    object(MyClass)#%d (%d) {
      ["id":"MyClass":private]=>
      string(3) "yip"
      ["child":"MyClass":private]=>
      object(MyClass)#%d (%d) {
        ["id":"MyClass":private]=>
        string(3) "yap"
        ["child":"MyClass":private]=>
        NULL
      }
    }
  }
}
===DONE===
