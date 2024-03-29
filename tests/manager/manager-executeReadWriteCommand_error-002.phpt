--TEST--
MongoDB\Driver\Manager::executeReadWriteCommand() cannot combine session with unacknowledged write concern
--SKIPIF--
<?php require __DIR__ . "/../utils/basic-skipif.inc"; ?>
<?php skip_if_not_libmongoc_crypto(); ?>
<?php skip_if_not_live(); ?>
--FILE--
<?php
require_once __DIR__ . "/../utils/basic.inc";

/* insert should not normally be used with executeReadWriteCommand(), but we are
 * only testing executeReadWriteCommand()'s option validation. */
echo throws(function() {
    $manager = create_test_manager();

    $command = new MongoDB\Driver\Command([
        'insert' => COLLECTION_NAME,
        'documents' => [['x' => 1]],
    ]);

    $manager->executeReadWriteCommand(DATABASE_NAME, $command, [
        'session' => $manager->startSession(),
        'writeConcern' => new MongoDB\Driver\WriteConcern(0),
    ]);
}, 'MongoDB\Driver\Exception\InvalidArgumentException'), "\n";

echo throws(function() {
    $manager = create_test_manager(URI, ['w' => 0]);

    $command = new MongoDB\Driver\Command([
        'insert' => COLLECTION_NAME,
        'documents' => [['x' => 1]],
    ]);

    $manager->executeReadWriteCommand(DATABASE_NAME, $command, [
        'session' => $manager->startSession(),
    ]);
}, 'MongoDB\Driver\Exception\InvalidArgumentException'), "\n";

?>
===DONE===
<?php exit(0); ?>
--EXPECT--
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Cannot combine "session" option with an unacknowledged write concern
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Cannot combine "session" option with an unacknowledged write concern
===DONE===
