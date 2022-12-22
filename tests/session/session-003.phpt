--TEST--
MongoDB\Driver\Session spec test: session cannot be used for different clients
--DESCRIPTION--
Session spec prose test #5: Session argument is for the right client
https://github.com/mongodb/specifications/blob/master/source/sessions/tests/README.rst#session-argument-is-for-the-right-client
--SKIPIF--
<?php require __DIR__ . "/../utils/basic-skipif.inc"; ?>
<?php skip_if_not_libmongoc_crypto(); ?>
<?php skip_if_not_live(); ?>
--FILE--
<?php
require_once __DIR__ . "/../utils/basic.inc";

$manager = create_test_manager(URI, [], ['disableClientPersistence' => true]);
$otherManager = create_test_manager(URI, [], ['disableClientPersistence' => true]);

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
