--TEST--
PHPC-1419: error labels from getMore are not exposed
--SKIPIF--
<?php require __DIR__ . "/../utils/basic-skipif.inc"; ?>
<?php skip_if_not_libmongoc_crypto(); ?>
<?php skip_if_no_failcommand_failpoint(); ?>
<?php skip_if_server_version('<', '4.1.11'); ?>
<?php skip_if_not_clean(); ?>
--FILE--
<?php
require_once __DIR__ . "/../utils/basic.inc";

$manager = new MongoDB\Driver\Manager(URI);

// Select a specific server for future operations to avoid mongos switching in sharded clusters
$server = $manager->selectServer(new \MongoDB\Driver\ReadPreference('primary'));

$bulk = new MongoDB\Driver\BulkWrite;
$bulk->insert(['_id' => 1]);
$bulk->insert(['_id' => 2]);
$bulk->insert(['_id' => 3]);
$manager->executeBulkWrite(NS, $bulk);

$cursor = $server->executeQuery(NS, new \MongoDB\Driver\Query([], ['batchSize' => 1]));
$iterator = new IteratorIterator($cursor);

configureTargetedFailPoint(
    $server,
    'failCommand',
    [ 'times' => 1] ,
    [ 'errorCode' => 280, 'failCommands' => ['getMore'] ]
);

try {
    $iterator->next();
} catch (\MongoDB\Driver\Exception\ServerException $e) {
    var_dump($e->hasErrorLabel('NonResumableChangeStreamError'));
}

?>
===DONE===
--EXPECT--
bool(true)
===DONE===
