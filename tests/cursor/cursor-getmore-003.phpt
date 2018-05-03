--TEST--
MongoDB\Driver\Cursor command result iteration with batchSize requiring getmore with full batches
--SKIPIF--
<?php require __DIR__ . "/../utils/basic-skipif.inc"; ?>
<?php skip_if_not_live(); ?>
<?php skip_if_not_clean(); ?>
--FILE--
<?php
require_once __DIR__ . "/../utils/basic.inc";

$manager = new MongoDB\Driver\Manager(URI);

$bulkWrite = new MongoDB\Driver\BulkWrite;

for ($i = 0; $i < 6; $i++) {
    $bulkWrite->insert(array('_id' => $i));
}

$writeResult = $manager->executeBulkWrite(NS, $bulkWrite);
printf("Inserted: %d\n", $writeResult->getInsertedCount());

$command = new MongoDB\Driver\Command(array(
    'aggregate' => COLLECTION_NAME,
    'pipeline' => array(
        array('$match' => new stdClass),
    ),
    'cursor' => array('batchSize' => 2),
));

$cursor = $manager->executeCommand(DATABASE_NAME, $command);

foreach ($cursor as $i => $document) {
    printf("%d => {_id: %d}\n", $i, $document->_id);
}

?>
===DONE===
<?php exit(0); ?>
--EXPECT--
Inserted: 6
0 => {_id: 0}
1 => {_id: 1}
2 => {_id: 2}
3 => {_id: 3}
4 => {_id: 4}
5 => {_id: 5}
===DONE===
