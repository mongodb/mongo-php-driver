--TEST--
MongoDB\Driver\Manager::executeCommand() cannot combine session with unacknowledged write concern
--SKIPIF--
<?php require __DIR__ . "/../utils/basic-skipif.inc"; ?>
<?php skip_if_not_libmongoc_crypto(); ?>
<?php skip_if_not_live(); ?>
<?php skip_if_server_version('<', '3.6'); ?>
--FILE--
<?php
require_once __DIR__ . "/../utils/basic.inc";
echo throws(function() {
    $manager = new MongoDB\Driver\Manager(URI);
    $command = new MongoDB\Driver\Command([
        'insert' => COLLECTION_NAME,
        'documents' => [['x' => 1]],
    ]);
    $manager->executeCommand(DATABASE_NAME, $command, [
        'session' => $manager->startSession(),
        'writeConcern' => new MongoDB\Driver\WriteConcern(0),
    ]);
}, 'MongoDB\Driver\Exception\InvalidArgumentException'), "\n";
?>
===DONE===
<?php exit(0); ?>
--EXPECT--
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Cannot combine "session" option with an unacknowledged write concern
===DONE===
