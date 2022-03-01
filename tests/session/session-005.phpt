--TEST--
MongoDB\Driver\Session spec test: snapshot option requires MongoDB 5.0+
--DESCRIPTION--
PHPC-1876: Raise client error for snapshot sessions on <5.0 servers
--SKIPIF--
<?php require __DIR__ . "/../utils/basic-skipif.inc"; ?>
<?php skip_if_not_libmongoc_crypto(); ?>
<?php skip_if_not_live(); ?>
<?php skip_if_server_version('>=', '5.0'); ?>
--FILE--
<?php
require_once __DIR__ . "/../utils/basic.inc";

$manager = create_test_manager();
$session = $manager->startSession(['snapshot' => true]);

/* Note: executeBulkWrite() always throws a BulkWriteException. Any previous
 * exception's message will be included in the BulkWriteException message. */
echo "\nTesting executeBulkWrite()\n";
echo throws(function() use ($manager, $session) {
    $bulk = new MongoDB\Driver\BulkWrite();
    $bulk->insert(['x' => 1]);
    $manager->executeBulkWrite(NS, $bulk, ['session' => $session]);
}, MongoDB\Driver\Exception\BulkWriteException::class), "\n";

echo "\nTesting executeCommand()\n";
echo throws(function() use ($manager, $session) {
    $command = new MongoDB\Driver\Command(['ping' => 1]);
    $manager->executeCommand(DATABASE_NAME, $command, ['session' => $session]);
}, MongoDB\Driver\Exception\RuntimeException::class), "\n";

echo "\nTesting executeQuery()\n";
echo throws(function() use ($manager, $session) {
    $query = new MongoDB\Driver\Query([]);
    $manager->executeQuery(NS, $query, ['session' => $session]);
}, MongoDB\Driver\Exception\RuntimeException::class), "\n";

?>
===DONE===
<?php exit(0); ?>
--EXPECT--
Testing executeBulkWrite()
OK: Got MongoDB\Driver\Exception\BulkWriteException
Bulk write failed due to previous MongoDB\Driver\Exception\RuntimeException: Snapshot reads require MongoDB 5.0 or later

Testing executeCommand()
OK: Got MongoDB\Driver\Exception\RuntimeException
Snapshot reads require MongoDB 5.0 or later

Testing executeQuery()
OK: Got MongoDB\Driver\Exception\RuntimeException
Snapshot reads require MongoDB 5.0 or later
===DONE===
