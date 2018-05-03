--TEST--
MongoDB\Driver\Manager::executeBulkWrite() cannot combine session with unacknowledged write concern
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

    $bulk = new MongoDB\Driver\BulkWrite;
    $bulk->insert(['x' => 1]);

    $manager->executeBulkWrite(NS, $bulk, [
        'session' => $manager->startSession(),
        'writeConcern' => new MongoDB\Driver\WriteConcern(0),
    ]);
}, 'MongoDB\Driver\Exception\InvalidArgumentException'), "\n";

echo throws(function() {
    $manager = new MongoDB\Driver\Manager(URI, ['w' => 0]);

    $bulk = new MongoDB\Driver\BulkWrite;
    $bulk->insert(['x' => 1]);

    $manager->executeBulkWrite(NS, $bulk, [
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
