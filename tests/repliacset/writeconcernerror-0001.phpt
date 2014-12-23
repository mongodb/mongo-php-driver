--TEST--
WriteConcernError: Access write counts and WriteConcern reason
--SKIPIF--
<?php require "tests/utils/basic-skipif.inc"?>
--FILE--
<?php 
require_once "tests/utils/basic.inc";

$mc = new MongoDB\Manager(MONGODB_REPLICASET_URI);

$batch = new MongoDB\WriteBatch;

$batch->insert(array("my" => "value"));
$batch->insert(array("my" => "value", "foo" => "bar"));
$batch->insert(array("my" => "value", "foo" => "bar"));

$batch->delete(array("my" => "value", "foo" => "bar"), array("limit" => 1));

$batch->update(array("foo" => "bar"), array('$set' => array("foo" => "baz")), array("limit" => 1, "upsert" => 0));

$w = new MongoDB\WriteConcern(30, 100);
$retval = $mc->executeWriteBatch(NS, $batch, $w);

printWriteResult($retval);
?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
server: %s:%d
insertedCount: 3
matchedCount: 1
modifiedCount: 1
upsertedCount: 0
deletedCount: 1
writeConcernError.message: waiting for replication timed out
writeConcernError.code: 64
writeConcernError.info: array(1) {
  ["wtimeout"]=>
  bool(true)
}
===DONE===
