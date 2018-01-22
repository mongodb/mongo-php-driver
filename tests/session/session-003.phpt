--TEST--
MongoDB\Driver\Session spec test: session cannot be used for different clients
--SKIPIF--
<?php require __DIR__ . "/../utils/basic-skipif.inc"; ?>
<?php NEEDS_CRYPTO(); ?>
<?php NEEDS('STANDALONE'); NEEDS_ATLEAST_MONGODB_VERSION(STANDALONE, "3.6"); CLEANUP(STANDALONE); ?>
--FILE--
<?php
require_once __DIR__ . "/../utils/basic.inc";

// Vary heartbeatFrequencyMS to ensure each Manager gets a different client
$manager = new MongoDB\Driver\Manager(STANDALONE, ['heartbeatFrequencyMS' => 60000]);
$otherManager = new MongoDB\Driver\Manager(STANDALONE, ['heartbeatFrequencyMS' => 90000]);

// Create a session with the second Manager (associated with different client)
$session = $otherManager->startSession();

echo "\nTesting executeBulkWrite()\n";
echo throws(function() use ($manager, $session) {
    $bulk = new MongoDB\Driver\BulkWrite();
    $bulk->insert(['x' => 1]);
    $manager->executeBulkWrite(NS, $bulk, ['session' => $session]);
}, 'MongoDB\Driver\Exception\InvalidArgumentException'), "\n";

echo "\nTesting executeCommand()\n";
echo throws(function() use ($manager, $session) {
    $command = new MongoDB\Driver\Command(['ping' => 1]);
    $manager->executeCommand(DATABASE_NAME, $command, ['session' => $session]);
}, 'MongoDB\Driver\Exception\InvalidArgumentException'), "\n";

echo "\nTesting executeQuery()\n";
echo throws(function() use ($manager, $session) {
    $query = new MongoDB\Driver\Query([]);
    $manager->executeQuery(NS, $query, ['session' => $session]);
}, 'MongoDB\Driver\Exception\InvalidArgumentException'), "\n";

?>
===DONE===
<?php exit(0); ?>
--EXPECT--
Testing executeBulkWrite()
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Cannot use Session started from a different Manager

Testing executeCommand()
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Cannot use Session started from a different Manager

Testing executeQuery()
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Cannot use Session started from a different Manager
===DONE===
