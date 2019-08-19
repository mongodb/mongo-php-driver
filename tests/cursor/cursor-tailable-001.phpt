--TEST--
MongoDB\Driver\Cursor tailable iteration
--SKIPIF--
<?php require __DIR__ . "/../utils/basic-skipif.inc"; ?>
<?php skip_if_not_live(); ?>
<?php skip_if_not_clean(); ?>
--FILE--
<?php
require_once __DIR__ . "/../utils/basic.inc";

function insert(MongoDB\Driver\Manager $manager, $from, $to = null)
{
    if ($to === null) {
        $to = $from;
    }

    $bulkWrite = new MongoDB\Driver\BulkWrite;

    for ($i = $from; $i <= $to; $i++) {
        $bulkWrite->insert(['_id' => $i]);
    }

    $writeResult = $manager->executeBulkWrite(NS, $bulkWrite);

    printf("Inserted %d document(s): %s\n", $writeResult->getInsertedCount(), implode(', ', range($from, $to)));
}

$manager = new MongoDB\Driver\Manager(URI);

$manager->executeCommand(DATABASE_NAME, new MongoDB\Driver\Command([
    'create' => COLLECTION_NAME,
    'capped' => true,
    'size' => 1048576,
]));

insert($manager, 1, 3);

$cursor = $manager->executeQuery(NS, new MongoDB\Driver\Query([], ['tailable' => true]));
$it = new IteratorIterator($cursor);

$numAwaitAttempts = 0;
$maxAwaitAttempts = 7;

for ($it->rewind(); $numAwaitAttempts < $maxAwaitAttempts; $it->next()) {
    $document = $it->current();

    if ($document !== null) {
        printf("{_id: %d}\n", $document->_id);
        continue;
    }

    if ($numAwaitAttempts === 2) {
        insert($manager, 4, 6);
    }

    if ($numAwaitAttempts === 5) {
        insert($manager, 7, 9);
    }

    echo "Awaiting results...\n";
    $numAwaitAttempts += 1;
}

?>
===DONE===
<?php exit(0); ?>
--EXPECT--
Inserted 3 document(s): 1, 2, 3
{_id: 1}
{_id: 2}
{_id: 3}
Awaiting results...
Awaiting results...
Inserted 3 document(s): 4, 5, 6
Awaiting results...
{_id: 4}
{_id: 5}
{_id: 6}
Awaiting results...
Awaiting results...
Inserted 3 document(s): 7, 8, 9
Awaiting results...
{_id: 7}
{_id: 8}
{_id: 9}
Awaiting results...
===DONE===
