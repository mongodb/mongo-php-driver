--TEST--
MongoDB\Driver\Manager::executeBulkWriteCommand() prohibits session and unacknowledged write concern (inherited)
--SKIPIF--
<?php require __DIR__ . "/../utils/basic-skipif.inc"; ?>
<?php skip_if_not_live(); ?>
<?php skip_if_server_version('<', '8.0'); ?>
<?php skip_if_not_clean(); ?>
--FILE--
<?php

require_once __DIR__ . "/../utils/basic.inc";

$manager = create_test_manager(URI, ['w' => 0]);

$bulk = new MongoDB\Driver\BulkWriteCommand();
$bulk->insertOne(NS, ['_id' => 1]);

echo throws(function() use ($manager, $bulk) {
    $manager->executeBulkWriteCommand($bulk, [
        'session' => $manager->startSession(),
    ]);
}, MongoDB\Driver\Exception\InvalidArgumentException::class), "\n";

?>
===DONE===
<?php exit(0); ?>
--EXPECT--
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Cannot combine "session" option with an unacknowledged write concern
===DONE===
