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

$manager = create_test_manager(URI, ['w' => 0]);

$bulk = new MongoDB\Driver\BulkWriteCommand(['ordered' => false]);
$bulk->insertOne(NS, ['_id' => 1]);

$result = $manager->executeBulkWriteCommand($bulk);

var_dump($result->isAcknowledged());

// Additionally test that other accessor methods cannot be called
echo throws(function() use ($result) {
    $result->getInsertedCount();
}, MongoDB\Driver\Exception\LogicException::class), "\n";

echo throws(function() use ($result) {
    $result->getMatchedCount();
}, MongoDB\Driver\Exception\LogicException::class), "\n";

echo throws(function() use ($result) {
    $result->getModifiedCount();
}, MongoDB\Driver\Exception\LogicException::class), "\n";

echo throws(function() use ($result) {
    $result->getUpsertedCount();
}, MongoDB\Driver\Exception\LogicException::class), "\n";

echo throws(function() use ($result) {
    $result->getDeletedCount();
}, MongoDB\Driver\Exception\LogicException::class), "\n";

echo throws(function() use ($result) {
    $result->getInsertResults();
}, MongoDB\Driver\Exception\LogicException::class), "\n";

echo throws(function() use ($result) {
    $result->getUpdateResults();
}, MongoDB\Driver\Exception\LogicException::class), "\n";

echo throws(function() use ($result) {
    $result->getDeleteResults();
}, MongoDB\Driver\Exception\LogicException::class), "\n";

?>
===DONE===
<?php exit(0); ?>
--EXPECT--
bool(false)
OK: Got MongoDB\Driver\Exception\LogicException
MongoDB\Driver\BulkWriteCommandResult::getInsertedCount() should not be called for an unacknowledged write result
OK: Got MongoDB\Driver\Exception\LogicException
MongoDB\Driver\BulkWriteCommandResult::getMatchedCount() should not be called for an unacknowledged write result
OK: Got MongoDB\Driver\Exception\LogicException
MongoDB\Driver\BulkWriteCommandResult::getModifiedCount() should not be called for an unacknowledged write result
OK: Got MongoDB\Driver\Exception\LogicException
MongoDB\Driver\BulkWriteCommandResult::getUpsertedCount() should not be called for an unacknowledged write result
OK: Got MongoDB\Driver\Exception\LogicException
MongoDB\Driver\BulkWriteCommandResult::getDeletedCount() should not be called for an unacknowledged write result
OK: Got MongoDB\Driver\Exception\LogicException
MongoDB\Driver\BulkWriteCommandResult::getInsertResults() should not be called for an unacknowledged write result
OK: Got MongoDB\Driver\Exception\LogicException
MongoDB\Driver\BulkWriteCommandResult::getUpdateResults() should not be called for an unacknowledged write result
OK: Got MongoDB\Driver\Exception\LogicException
MongoDB\Driver\BulkWriteCommandResult::getDeleteResults() should not be called for an unacknowledged write result
===DONE===
