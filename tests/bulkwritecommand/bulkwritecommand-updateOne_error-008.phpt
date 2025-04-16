--TEST--
MongoDB\Driver\BulkWriteCommand::updateOne() hint option requires MongoDB 4.2 (server-side error)
--FILE--
<?php
require_once __DIR__ . "/../utils/basic.inc";

$manager = create_test_manager();

$bulk = new MongoDB\Driver\BulkWriteCommand;
$bulk->updateOne(NS, ['_id' => 1], ['$set' => ['x' => 11]], ['hint' => '_id_']);

// @TODO: FAILED: Expected MongoDB\Driver\Exception\BulkWriteException, but no exception thrown!
echo throws(function() use ($manager, $bulk) {
    $manager->executeBulkWriteCommand($bulk);
}, 'MongoDB\Driver\Exception\BulkWriteException'), "\n";

?>
===DONE===
<?php exit(0); ?>
--EXPECT--
OK: Got MongoDB\Driver\Exception\BulkWriteException
BSON field 'update.updates.hint' is an unknown field.
===DONE===
