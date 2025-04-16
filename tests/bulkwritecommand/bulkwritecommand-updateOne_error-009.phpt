--TEST--
MongoDB\Driver\BulkWriteCommand::updateOne() with multi:true prohibits sort option
--FILE--
<?php
require_once __DIR__ . "/../utils/basic.inc";

$manager = create_test_manager();
$bulk = new MongoDB\Driver\BulkWriteCommand;

// @TODO: FAILED: Expected MongoDB\Driver\Exception\InvalidArgumentException, but no exception thrown!
echo throws(function() use ($manager, $bulk) {
    $bulk->updateOne(NS, ['x' => ['$gt' => 1]], ['$set' => ['y' => 11]], ['multi' => true, 'sort' => ['x' => 1]]);
}, MongoDB\Driver\Exception\InvalidArgumentException::class), "\n";

$manager->executeBulkWriteCommand($bulk);

?>
===DONE===
<?php exit(0); ?>
--EXPECT--
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Invalid option 'sort'
===DONE===
