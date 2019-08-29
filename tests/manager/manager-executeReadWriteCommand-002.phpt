--TEST--
MongoDB\Driver\Manager::executeReadWriteCommand() pins transaction to server
--SKIPIF--
<?php require __DIR__ . "/../utils/basic-skipif.inc"; ?>
<?php skip_if_not_mongos_with_replica_set(); ?>
<?php skip_if_server_version('<', '4.1.6'); ?>
<?php skip_if_not_clean(); ?>
--FILE--
<?php
require_once __DIR__ . "/../utils/basic.inc";

$manager = new MongoDB\Driver\Manager(URI);

/* Create collections as that can't be (automatically) done in a transaction */
$manager->executeReadWriteCommand(
    DATABASE_NAME,
    new \MongoDB\Driver\Command([ 'create' => COLLECTION_NAME ]),
    [ 'writeConcern' => new \MongoDB\Driver\WriteConcern( \MongoDB\Driver\WriteConcern::MAJORITY ) ]
);

$session = $manager->startSession();
var_dump($session->getServer() instanceof \MongoDB\Driver\Server);

$session->startTransaction();
var_dump($session->getServer() instanceof \MongoDB\Driver\Server);

$command = new MongoDB\Driver\Command([
    'aggregate' => COLLECTION_NAME,
    'pipeline' => [['$group' => ['_id' => 1]]],
    'cursor' => (object) []
]);
$manager->executeReadWriteCommand(DATABASE_NAME, $command, ['session' => $session]);

$pinnedServer = $session->getServer();
var_dump($pinnedServer instanceof \MongoDB\Driver\Server);

$bulk = new MongoDB\Driver\BulkWrite();
$bulk->insert(['x' => 1]);
$manager->executeBulkWrite(NS, $bulk, ['session' => $session]);

$session->commitTransaction();

var_dump($session->getServer() == $pinnedServer);

$bulk = new MongoDB\Driver\BulkWrite();
$bulk->insert(['x' => 1]);
$manager->executeBulkWrite(NS, $bulk, ['session' => $session]);

var_dump($session->getServer() instanceof \MongoDB\Driver\Server);

?>
===DONE===
<?php exit(0); ?>
--EXPECT--
bool(false)
bool(false)
bool(true)
bool(true)
bool(false)
===DONE===
