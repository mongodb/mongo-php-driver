--TEST--
MongoDB\Driver\Server::executeBulkWrite() pins transaction to server
--SKIPIF--
<?php require __DIR__ . "/../utils/basic-skipif.inc"; ?>
<?php skip_if_not_mongos(); ?>
<?php skip_if_no_transactions(); ?>
<?php skip_if_not_clean(); ?>
--FILE--
<?php
require_once __DIR__ . "/../utils/basic.inc";

$manager = create_test_manager();

/* Create collections as that can't be (automatically) done in a transaction */
$manager->executeCommand(
    DATABASE_NAME,
    new \MongoDB\Driver\Command([ 'create' => COLLECTION_NAME ]),
    [ 'writeConcern' => new \MongoDB\Driver\WriteConcern( \MongoDB\Driver\WriteConcern::MAJORITY ) ]
);

$servers = $manager->getServers();
$selectedServer = array_pop($servers);
$wrongServer = array_pop($servers);
var_dump($selectedServer != $wrongServer);

$session = $manager->startSession();
var_dump($session->getServer() instanceof \MongoDB\Driver\Server);

$session->startTransaction();
var_dump($session->getServer() instanceof \MongoDB\Driver\Server);

$bulk = new MongoDB\Driver\BulkWrite();
$bulk->insert(['x' => 1]);
$selectedServer->executeBulkWrite(NS, $bulk, ['session' => $session]);

var_dump($session->getServer() == $selectedServer);

$bulk = new MongoDB\Driver\BulkWrite();
$bulk->insert(['x' => 1]);
$selectedServer->executeBulkWrite(NS, $bulk, ['session' => $session]);

echo throws(function () use ($wrongServer, $session) {
    $bulk = new MongoDB\Driver\BulkWrite();
    $bulk->insert(['x' => 1]);
    $wrongServer->executeBulkWrite(NS, $bulk, ['session' => $session]);
}, \MongoDB\Driver\Exception\BulkWriteException::class), "\n";

$session->commitTransaction();

var_dump($session->getServer() == $selectedServer);

$bulk = new MongoDB\Driver\BulkWrite();
$bulk->insert(['x' => 1]);
$selectedServer->executeBulkWrite(NS, $bulk, ['session' => $session]);

var_dump($session->getServer() instanceof \MongoDB\Driver\Server);

?>
===DONE===
<?php exit(0); ?>
--EXPECT--
bool(true)
bool(false)
bool(false)
bool(true)
OK: Got MongoDB\Driver\Exception\BulkWriteException
Bulk write failed due to previous MongoDB\Driver\Exception\RuntimeException: Requested server id does not matched pinned server id
bool(true)
bool(false)
===DONE===
