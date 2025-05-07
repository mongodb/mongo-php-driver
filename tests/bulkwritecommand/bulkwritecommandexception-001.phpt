--TEST--
MongoDB\Driver\BulkWriteCommandResult::isAcknowledged() with unacknowledged write concern
--SKIPIF--
<?php require __DIR__ . "/../utils/basic-skipif.inc"; ?>
<?php skip_if_not_live(); ?>
<?php skip_if_server_version('<', '8.0'); ?>
<?php skip_if_not_clean(); ?>
--FILE--
<?php

require_once __DIR__ . "/../utils/basic.inc";

$manager = create_test_manager(URI);

$bulk = new MongoDB\Driver\BulkWriteCommand(['ordered' => false]);
$bulk->insertOne(NS, ['_id' => 1]);
$bulk->insertOne(NS, ['_id' => 1]);

echo throws(function() use ($result) {
    $manager->executeBulkWriteCommand($bulk);
}, MongoDB\Driver\Exception\BulkWriteCommandException::class), "\n";

?>
===DONE===
<?php exit(0); ?>
--EXPECT--
bool(false)
OK: Got MongoDB\Driver\Exception\BulkWriteCommandException
Bulk write failed with 0 write concern errors and 1 write errors
===DONE===
