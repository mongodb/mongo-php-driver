--TEST--
MongoDB\Driver\Manager::executeBulkWrite() BulkWriteException inherits labels from previous exception
--SKIPIF--
<?php require __DIR__ . "/../utils/basic-skipif.inc"; ?>
<?php skip_if_not_libmongoc_crypto(); ?>
<?php skip_if_not_replica_set_or_mongos_with_replica_set(); ?>
<?php skip_if_server_version('<', '4.0'); ?>
<?php skip_if_no_failcommand_failpoint(); ?>
<?php skip_if_not_clean(); ?>
--FILE--
<?php
require_once __DIR__ . "/../utils/basic.inc";

$manager = new MongoDB\Driver\Manager(URI);

// Select a specific server for future operations to avoid mongos switching in sharded clusters
$server = $manager->selectServer(new \MongoDB\Driver\ReadPreference('primary'));

// Create collection since it can't be (automatically) done within the transaction
$majority = new MongoDB\Driver\WriteConcern(MongoDB\Driver\WriteConcern::MAJORITY);
$server->executeWriteCommand(
    DATABASE_NAME,
    new MongoDB\Driver\Command(['create' => COLLECTION_NAME]),
    ['writeConcern' => $majority]
);

configureTargetedFailPoint($server, 'failCommand', [ 'times' => 1 ], [
  'failCommands' => ['insert'],
  'closeConnection' => true,
]);

$session = $manager->startSession();
$session->startTransaction();

$bulk = new MongoDB\Driver\BulkWrite;
$bulk->insert(['x' => 1]);

try {
    $server->executeBulkWrite(NS, $bulk, ['session' => $session]);
} catch (MongoDB\Driver\Exception\BulkWriteException $e) {
    printf("%s(%d): %s\n", get_class($e), $e->getCode(), $e->getMessage());
    var_dump($e->hasErrorLabel('TransientTransactionError'));
    $prev = $e->getPrevious();
    printf("%s(%d): %s\n", get_class($prev), $prev->getCode(), $prev->getMessage());
    var_dump($prev->hasErrorLabel('TransientTransactionError'));
}

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
MongoDB\Driver\Exception\BulkWriteException(0): Bulk write failed due to previous MongoDB\Driver\Exception\ConnectionTimeoutException: Failed to send "insert" command with database "%s": Failed to read 4 bytes: socket error or timeout
bool(true)
MongoDB\Driver\Exception\ConnectionTimeoutException(%d): Failed to send "insert" command with database "%s": Failed to read 4 bytes: socket error or timeout
bool(true)
===DONE===
