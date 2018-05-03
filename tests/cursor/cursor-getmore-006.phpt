--TEST--
MongoDB\Driver\Cursor command result iteration with getmore failure
--XFAIL--
START() tests must be reimplemented (PHPC-1179)
--SKIPIF--
<?php require __DIR__ . "/" ."../utils/basic-skipif.inc"; ?>
<?php START("THROWAWAY", ["version" => "30-release"]); CLEANUP(THROWAWAY); ?>
--FILE--
<?php
require_once __DIR__ . "/../utils/basic.inc";

$manager = new MongoDB\Driver\Manager(THROWAWAY);

$bulkWrite = new MongoDB\Driver\BulkWrite;

for ($i = 0; $i < 5; $i++) {
    $bulkWrite->insert(array('_id' => $i));
}

$writeResult = $manager->executeBulkWrite(NS, $bulkWrite);
printf("Inserted: %d\n", $writeResult->getInsertedCount());

$command = new MongoDB\Driver\Command([
    'aggregate' => COLLECTION_NAME,
    'pipeline' => [
        ['$match' => new stdClass],
    ],
    'cursor' => ['batchSize' => 2],
]);

$cursor = $manager->executeCommand(DATABASE_NAME, $command);

failGetMore($manager);

throws(function() use ($cursor) {
    foreach ($cursor as $i => $document) {
        printf("%d => {_id: %d}\n", $i, $document->_id);
    }
}, "MongoDB\Driver\Exception\ConnectionException");

?>
===DONE===
<?php DELETE("THROWAWAY"); ?>
<?php exit(0); ?>
--CLEAN--
<?php require __DIR__ . "/../utils/basic-skipif.inc"; ?>
<?php DELETE("THROWAWAY"); ?>
--EXPECT--
Inserted: 5
0 => {_id: 0}
1 => {_id: 1}
OK: Got MongoDB\Driver\Exception\ConnectionException
===DONE===
