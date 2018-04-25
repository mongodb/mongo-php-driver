--TEST--
MongoDB\Driver\Cursor cannot rewind after starting iteration
--SKIPIF--
<?php require __DIR__ . "/../utils/basic-skipif.inc"; ?>
<?php skip_if_not_live(); ?>
<?php skip_if_not_clean(); ?>
--FILE--
<?php
require_once __DIR__ . "/../utils/basic.inc";

class MyIteratorIterator extends IteratorIterator
{
    private $name;

    public function __construct(Traversable $iterator, $name)
    {
        parent::__construct($iterator);
        $this->name = (string) $name;
    }

    public function dump()
    {
        $key = parent::key();
        $current = parent::current();
        $position = is_int($key) ? (string) $key : 'null';
        $document = is_object($current) ? sprintf("{_id: %d}", $current->_id) : 'null';
        printf("%s: %s => %s\n", $this->name, $position, $document);
    }
}

$manager = new MongoDB\Driver\Manager(URI);

$bulkWrite = new MongoDB\Driver\BulkWrite;

for ($i = 0; $i < 5; $i++) {
    $bulkWrite->insert(array('_id' => $i));
}

$writeResult = $manager->executeBulkWrite(NS, $bulkWrite);
printf("Inserted: %d\n", $writeResult->getInsertedCount());

$cursor = $manager->executeQuery(NS, new MongoDB\Driver\Query(array()));
$a = new MyIteratorIterator($cursor, 'A');

echo "\nRewinding sets the current element:\n";
$a->rewind();
$a->dump();

echo "\nRewinding again is OK since we haven't advanced:\n";
$a->rewind();
$a->dump();

echo "\nAdvancing populates the next element:\n";
$a->next();
$a->dump();

echo "\nRewinding after advancing is not OK:\n";
try {
    $a->rewind();
    echo "FAILED: rewind should throw if iteration has started\n";
} catch (MongoDB\Driver\Exception\LogicException $e) {
    printf("LogicException: %s\n", $e->getMessage());
}

echo "\nAdvancing through remaining elements:\n";
$a->next();
$a->dump();
$a->next();
$a->dump();
$a->next();
$a->dump();

echo "\nAdvancing beyond the last element:\n";
$a->next();
$a->dump();

?>
===DONE===
<?php exit(0); ?>
--EXPECT--
Inserted: 5

Rewinding sets the current element:
A: 0 => {_id: 0}

Rewinding again is OK since we haven't advanced:
A: 0 => {_id: 0}

Advancing populates the next element:
A: 1 => {_id: 1}

Rewinding after advancing is not OK:
LogicException: Cursors cannot rewind after starting iteration

Advancing through remaining elements:
A: 2 => {_id: 2}
A: 3 => {_id: 3}
A: 4 => {_id: 4}

Advancing beyond the last element:
A: null => null
===DONE===
