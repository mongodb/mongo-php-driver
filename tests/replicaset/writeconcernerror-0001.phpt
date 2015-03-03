--TEST--
WriteConcernError: Access write counts and WriteConcern reason
--SKIPIF--
<?php require "tests/utils/basic-skipif.inc"?>
--FILE--
<?php
require_once "tests/utils/basic.inc";

$manager = new MongoDB\Driver\Manager(MONGODB_REPLICASET_URI);

$bulk = new MongoDB\Driver\BulkWrite;

$bulk->insert(array("my" => "value"));
$bulk->insert(array("my" => "value", "foo" => "bar"));
$bulk->insert(array("my" => "value", "foo" => "bar"));

$bulk->delete(array("my" => "value", "foo" => "bar"), array("limit" => 1));

$bulk->update(array("foo" => "bar"), array('$set' => array("foo" => "baz")), array("limit" => 1, "upsert" => 0));

$w = new MongoDB\Driver\WriteConcern(30, 100);
try {
    $retval = $manager->executeBulkWrite(NS, $bulk, $w);
} catch(MongoDB\Driver\BulkWriteException $e) {
    printWriteResult($e->getWriteResult());
}
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
object(MongoDB\Driver\WriteConcernError)#%d (%d) {
  ["message"]=>
  string(33) "waiting for replication timed out"
  ["code"]=>
  int(64)
  ["info"]=>
  array(1) {
    ["wtimeout"]=>
    bool(true)
  }
}
writeConcernError.message: waiting for replication timed out
writeConcernError.code: 64
writeConcernError.info: array(1) {
  ["wtimeout"]=>
  bool(true)
}
===DONE===
