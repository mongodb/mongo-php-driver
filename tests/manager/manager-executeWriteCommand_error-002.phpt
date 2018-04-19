--TEST--
MongoDB\Driver\Manager::executeWriteCommand() cannot combine session with unacknowledged write concern
--SKIPIF--
<?php require __DIR__ . "/../utils/basic-skipif.inc"; ?>
<?php NEEDS_CRYPTO(); ?>
<?php NEEDS('STANDALONE'); ?>
<?php NEEDS_ATLEAST_MONGODB_VERSION(STANDALONE, "3.6"); ?>
--FILE--
<?php
require_once __DIR__ . "/../utils/basic.inc";

echo throws(function() {
    $manager = new MongoDB\Driver\Manager(STANDALONE);

    $command = new MongoDB\Driver\Command([
        'insert' => COLLECTION_NAME,
        'documents' => [['x' => 1]],
    ]);

    $manager->executeWriteCommand(DATABASE_NAME, $command, [
        'session' => $manager->startSession(),
        'writeConcern' => new MongoDB\Driver\WriteConcern(0),
    ]);
}, 'MongoDB\Driver\Exception\InvalidArgumentException'), "\n";

echo throws(function() {
    $manager = new MongoDB\Driver\Manager(STANDALONE, ['w' => 0]);

    $command = new MongoDB\Driver\Command([
        'insert' => COLLECTION_NAME,
        'documents' => [['x' => 1]],
    ]);

    $manager->executeWriteCommand(DATABASE_NAME, $command, [
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
