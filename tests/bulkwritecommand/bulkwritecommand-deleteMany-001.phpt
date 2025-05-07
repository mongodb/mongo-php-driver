--TEST--
MongoDB\Driver\BulkWriteCommand::deleteMany() should always encode __pclass for Persistable objects
--SKIPIF--
<?php require __DIR__ . "/../utils/basic-skipif.inc"; ?>
<?php skip_if_not_live(); ?>
<?php skip_if_server_version('<', '8.0'); ?>
<?php skip_if_not_clean(); ?>
--FILE--
<?php
require_once __DIR__ . "/../utils/basic.inc";

class MyClass implements MongoDB\BSON\Persistable
{
    private $id;
    private $child;

    public function __construct($id, ?MyClass $child = null)
    {
        $this->id = $id;
        $this->child = $child;
    }

    public function bsonSerialize(): array
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

$bulk = new MongoDB\Driver\BulkWriteCommand();
$bulk->insertOne(NS, $document);
$bulk->insertOne(NS, new MyClass('foo2', new MyClass('bar', new MyClass('baz'))));
$bulk->insertOne(NS, new MyClass('foo3', new MyClass('bar', new MyClass('baz'))));
$result = $manager->executeBulkWriteCommand($bulk);
printf("Inserted %d document(s)\n", $result->getInsertedCount());

$cursor = $manager->executeQuery(NS, new MongoDB\Driver\Query([]));
var_dump(count($cursor->toArray()));

$bulk = new MongoDB\Driver\BulkWriteCommand();
$bulk->deleteMany(NS, $document);
$result = $manager->executeBulkWriteCommand($bulk);
printf("Deleted %d document(s)\n", $result->getDeletedCount());

$cursor = $manager->executeQuery(NS, new MongoDB\Driver\Query([]));
var_dump(count($cursor->toArray()));

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
Inserted 3 document(s)
int(3)
Deleted 1 document(s)
int(2)
===DONE===
