--TEST--
MongoDB\Driver\Cursor query result iteration with getmore failure
--SKIPIF--
<?php require __DIR__ . "/" ."../utils/basic-skipif.inc"; ?>
<?php
/* The failCommand failpoint is scoped to a single getMore, so this test can run
 * against the configured standalone server. */
?>
<?php skip_if_not_live(); ?>
<?php skip_if_not_standalone(); ?>
<?php skip_if_no_failcommand_failpoint(); ?>
<?php skip_if_auth(); ?>
--FILE--
<?php
require_once __DIR__ . "/../utils/basic.inc";

$manager = create_test_manager();

$bulkWrite = new MongoDB\Driver\BulkWrite;

for ($i = 0; $i < 5; $i++) {
    $bulkWrite->insert(array('_id' => $i));
}

$writeResult = $manager->executeBulkWrite(NS, $bulkWrite);
printf("Inserted: %d\n", $writeResult->getInsertedCount());

$query = new MongoDB\Driver\Query([], ['batchSize' => 2]);
$cursor = $manager->executeQuery(NS, $query);

failGetMore($manager);

throws(function() use ($cursor) {
    foreach ($cursor as $i => $document) {
        printf("%d => {_id: %d}\n", $i, $document->_id);
    }
}, MongoDB\Driver\Exception\ServerException::class);
?>
===DONE===
--EXPECT--
Inserted: 5
0 => {_id: 0}
1 => {_id: 1}
OK: Got MongoDB\Driver\Exception\ServerException
===DONE===
