--TEST--
MongoDB\Driver\Command construction should always encode __pclass for Persistable objects
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

    #[\ReturnTypeWillChange]
    public function bsonSerialize()
    {
        return [
            '_id' => $this->id,
            'child' => $this->child,
        ];
    }

    public function bsonUnserialize(array $data): void
    {
        $this->id = $data['_id'];
        $this->child = $data['child'];
    }
}

$manager = create_test_manager();

$document = new MyClass('foo', new MyClass('bar', new MyClass('baz')));

$cursor = $manager->executeCommand(DATABASE_NAME, new MongoDB\Driver\Command([
    'findAndModify' => COLLECTION_NAME,
    'query' => ['_id' => 'foo'],
    'update' => $document,
    'upsert' => true,
    'new' => true,
]));
var_dump($cursor->toArray()[0]->value);

$cursor = $manager->executeCommand(DATABASE_NAME, new MongoDB\Driver\Command([
    'aggregate' => COLLECTION_NAME,
    'pipeline' => [
        ['$match' => $document],
    ],
    'cursor' => new stdClass(),
]));
var_dump($cursor->toArray()[0]);

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
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
===DONE===
