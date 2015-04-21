--TEST--
MongoDB\Driver\Cursor query result shared by multiple iterators
--SKIPIF--
<?php require __DIR__ . "/../utils/basic-skipif.inc"; CLEANUP(STANDALONE) ?>
--FILE--
<?php
require_once __DIR__ . "/../utils/basic.inc";

$manager = new MongoDB\Driver\Manager(STANDALONE);

$bulkWrite = new MongoDB\Driver\BulkWrite;

for ($i = 0; $i < 8; $i++) {
    $bulkWrite->insert(array('_id' => $i));
}

$writeResult = $manager->executeBulkWrite(NS, $bulkWrite);
printf("Inserted: %d\n", $writeResult->getInsertedCount());

$cursor = $manager->executeQuery(NS, new MongoDB\Driver\Query(array()));

foreach ($cursor as $i => $document) {
    printf("A: %d => {_id: %d}\n", $i, $document['_id']);

    if ($i == 2) {
        foreach ($cursor as $j => $document) {
            printf("B: %d => {_id: %d}\n", $j, $document['_id']);

            if ($j == 2) {
                break;
            }
        }
    }
}

?>
===DONE===
<?php exit(0); ?>
--EXPECT--
Inserted: 8
A: 0 => {_id: 0}
A: 1 => {_id: 1}
A: 2 => {_id: 2}
B: 0 => {_id: 2}
B: 1 => {_id: 3}
B: 2 => {_id: 4}
A: 3 => {_id: 5}
A: 4 => {_id: 6}
A: 5 => {_id: 7}
===DONE===
