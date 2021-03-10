--TEST--
MongoDB\Driver\Exception\BulkWriteException::hasErrorLabel() with writeConcernError
--SKIPIF--
<?php require __DIR__ . "/../utils/basic-skipif.inc"; ?>
<?php skip_if_not_live(); ?>
<?php skip_if_no_failcommand_failpoint(); ?>
<?php skip_if_not_clean(); ?>
--FILE--
<?php
require_once __DIR__ . "/../utils/basic.inc";

// Disable retryWrites since we want to check for a RetryableWriteError error label
$manager = create_test_manager(URI, ['retryWrites' => false]);

// Select a specific server for future operations to avoid mongos switching in sharded clusters
$server = $manager->selectServer(new \MongoDB\Driver\ReadPreference('primary'));

configureTargetedFailPoint($server, 'failCommand', [ 'times' => 1 ], [
    'failCommands' => ['insert'],
    'writeConcernError' => [
        'code' => 91,
        'errmsg' => 'Replication is being shut down',
        'errorLabels' => ['RetryableWriteError'],
    ],
]);

$bulk = new MongoDB\Driver\BulkWrite;
$bulk->insert(['x' => 1]);

try {
    $server->executeBulkWrite(NS, $bulk);
} catch (MongoDB\Driver\Exception\BulkWriteException $e) {
    var_dump($e->hasErrorLabel('RetryableWriteError'));
}

?>
===DONE===
<?php exit(0); ?>
--EXPECT--
bool(true)
===DONE===
