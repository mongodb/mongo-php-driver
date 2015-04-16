--TEST--
MongoDB\Driver\Cursor iterator handlers
--SKIPIF--
<?php require __DIR__ . "/../utils/basic-skipif.inc"; CLEANUP(STANDALONE) ?>
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
        $document = is_array($current) ? sprintf("{_id: %d}", $current['_id']) : 'null';
        printf("%s: %s => %s\n", $this->name, $position, $document);
    }
}

$manager = new MongoDB\Driver\Manager(STANDALONE);

$bulkWrite = new MongoDB\Driver\BulkWrite;

for ($i = 0; $i < 5; $i++) {
    $bulkWrite->insert(array('_id' => $i));
}

$writeResult = $manager->executeBulkWrite(NS, $bulkWrite);
printf("Inserted: %d\n", $writeResult->getInsertedCount());

$cursor = $manager->executeQuery(NS, new MongoDB\Driver\Query(array()));
$a = new MyIteratorIterator($cursor, 'A');
$b = new MyIteratorIterator($cursor, 'B');

echo "\nBefore rewinding, position and current element are not populated:\n";
$a->dump();
$b->dump();

echo "\nAfter rewinding, current element is populated:\n";
$a->rewind();
$b->rewind();
$a->dump();
$b->dump();

echo "\nMultiple iterators have their own position, but share the same Cursor buffer:\n";
$a->next();
$a->dump();
$b->next();
$b->dump();
$a->next();
$a->dump();

echo "\nRewinding only populates current element and does not alter position:\n";
$a->rewind();
$a->dump();
$b->rewind();
$b->dump();

echo "\nAdvancing first iterator until end of shared Cursor buffer is reached:\n";
$a->next();
$a->dump();
$a->next();
$a->dump();

echo "\nRewinding second iterator to position it at end of shared Cursor buffer:\n";
$b->rewind();
$b->dump();

?>
===DONE===
<?php exit(0); ?>
--EXPECT--
Inserted: 5

Before rewinding, position and current element are not populated:
A: null => null
B: null => null

After rewinding, current element is populated:
A: 0 => {_id: 0}
B: 0 => {_id: 0}

Multiple iterators have their own position, but share the same Cursor buffer:
A: 1 => {_id: 1}
B: 1 => {_id: 2}
A: 2 => {_id: 3}

Rewinding only populates current element and does not alter position:
A: 2 => {_id: 3}
B: 1 => {_id: 3}

Advancing first iterator until end of shared Cursor buffer is reached:
A: 3 => {_id: 4}
A: null => null

Rewinding second iterator to position it at end of shared Cursor buffer:
B: null => null
===DONE===
