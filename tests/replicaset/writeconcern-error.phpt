--TEST--
WriteConcernError: Populate WriteConcernError on WriteConcern errors
--SKIPIF--
<?php require "tests/utils/basic-skipif.inc"?>
--FILE--
<?php
require_once "tests/utils/basic.inc";

$manager = new MongoDB\Driver\Manager(MONGODB_REPLICASET_URI);

$bulk = new MongoDB\Driver\BulkWrite;

$bulk->insert(array("my" => "value"));

$w = new MongoDB\Driver\WriteConcern(30, 100);
try {
    $retval = $manager->executeBulkWrite(NS, $bulk, $w);
} catch(MongoDB\Driver\BulkWriteException $e) {
    printWriteResult($e->getWriteResult(), false);
}
?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
server: %s:%d
insertedCount: 1
matchedCount: 0
modifiedCount: 0
upsertedCount: 0
deletedCount: 0
writeConcernError: %s (%d)
===DONE===
