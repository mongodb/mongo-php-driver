--TEST--
MongoDB\Driver\Cursor query result iteration with getmore failure
--SKIPIF--
<?php require __DIR__ . "/" ."../utils/basic-skipif.inc"; ?>
<?php
/* This test spins up its own mongod instance, so only run this in the most default "standalone, no
 * auth" configurations. This way, we can test on multiple server versions, but not waste resources
 * on f.e. Travis. */
?>
<?php skip_if_not_live(); ?>
<?php skip_if_not_standalone(); ?>
<?php skip_if_server_version(">=", "3.6"); ?>
<?php skip_if_no_getmore_failpoint(); ?>
<?php skip_if_auth(); ?>
<?php skip_if_no_mongo_orchestration(); ?>
--FILE--
<?php
require_once __DIR__ . "/../utils/basic.inc";

$uri = createTemporaryMongoInstance();
$manager = new MongoDB\Driver\Manager($uri);

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
}, "MongoDB\Driver\Exception\ConnectionException");
?>
===DONE===
<?php destroyTemporaryMongoInstance(); ?>
<?php exit(0); ?>
--CLEAN--
<?php require __DIR__ . "/../utils/basic-skipif.inc"; ?>
<?php destroyTemporaryMongoInstance(); ?>
--EXPECT--
Inserted: 5
0 => {_id: 0}
1 => {_id: 1}
OK: Got MongoDB\Driver\Exception\ConnectionException
===DONE===
