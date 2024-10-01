--TEST--
MongoDB\Driver\Manager::executeBulkWrite() WriteResult accessible for network error
--SKIPIF--
<?php require __DIR__ . "/../utils/basic-skipif.inc"; ?>
<?php skip_if_not_live(); ?>
<?php skip_if_no_failcommand_failpoint(); ?>
<?php skip_if_not_clean(); ?>
--FILE--
<?php
require_once __DIR__ . "/../utils/basic.inc";

$manager = create_test_manager();

// Select a specific server for future operations to avoid mongos switching in sharded clusters
$server = $manager->selectServer(new \MongoDB\Driver\ReadPreference('primary'));

configureTargetedFailPoint($server, 'failCommand', [ 'times' => 1 ], [
  'failCommands' => ['delete'],
  'closeConnection' => true,
]);

$bulk = new MongoDB\Driver\BulkWrite();
$bulk->insert(['x' => 1]);
$bulk->update(['x' => 1], ['$set' => ['y' => 1]]);
$bulk->delete(['x' => 1]);

echo throws(function() use ($server, $bulk) {
    $server->executeBulkWrite(NS, $bulk);
}, MongoDB\Driver\Exception\ConnectionTimeoutException::class), "\n";

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
OK: Got MongoDB\Driver\Exception\ConnectionTimeoutException
Failed to send "delete" command with database "%s": Failed to read 4 bytes: socket error or timeout
===DONE===
