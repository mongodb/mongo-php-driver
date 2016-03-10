--TEST--
MongoDB\Driver\WriteResult debug output with errors
--SKIPIF--
<?php require __DIR__ . "/../utils/basic-skipif.inc"; CLEANUP(REPLICASET); ?>
--FILE--
<?php
require_once __DIR__ . "/../utils/basic.inc";

$manager = new MongoDB\Driver\Manager(REPLICASET);

$bulk = new MongoDB\Driver\BulkWrite(['ordered' => false]);
$bulk->update(['x' => 1], ['$set' => ['y' => 1]], ['upsert' => true]);
$bulk->update(['x' => 2], ['$set' => ['y' => 2]], ['upsert' => true]);
$bulk->insert(['_id' => 1]);
$bulk->insert(['_id' => 1]);
$bulk->insert(['_id' => 2]);
$bulk->insert(['_id' => 2]);
$bulk->insert(['_id' => 3]);
$bulk->insert(['_id' => 3]);

try {
    $result = $manager->executeBulkWrite(NS, $bulk, new MongoDB\Driver\WriteConcern(30));
} catch (MongoDB\Driver\Exception\BulkWriteException $e) {
    var_dump($e->getWriteResult());
}

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
object(MongoDB\Driver\WriteResult)#%d (%d) {
  ["nInserted"]=>
  int(3)
  ["nMatched"]=>
  int(0)
  ["nModified"]=>
  int(0)
  ["nRemoved"]=>
  int(0)
  ["nUpserted"]=>
  int(2)
  ["upsertedIds"]=>
  array(2) {
    [0]=>
    array(2) {
      ["index"]=>
      int(0)
      ["_id"]=>
      object(MongoDB\BSON\ObjectID)#%d (%d) {
        ["oid"]=>
        string(24) "%x"
      }
    }
    [1]=>
    array(2) {
      ["index"]=>
      int(1)
      ["_id"]=>
      object(MongoDB\BSON\ObjectID)#%d (%d) {
        ["oid"]=>
        string(24) "%x"
      }
    }
  }
  ["writeErrors"]=>
  array(3) {
    [0]=>
    array(3) {
      ["index"]=>
      int(3)
      ["code"]=>
      int(11000)
      ["errmsg"]=>
      string(108) "E11000 duplicate key error collection: phongo.writeResult_writeresult_debug_002 index: _id_ dup key: { : 1 }"
    }
    [1]=>
    array(3) {
      ["index"]=>
      int(5)
      ["code"]=>
      int(11000)
      ["errmsg"]=>
      string(108) "E11000 duplicate key error collection: phongo.writeResult_writeresult_debug_002 index: _id_ dup key: { : 2 }"
    }
    [2]=>
    array(3) {
      ["index"]=>
      int(7)
      ["code"]=>
      int(11000)
      ["errmsg"]=>
      string(108) "E11000 duplicate key error collection: phongo.writeResult_writeresult_debug_002 index: _id_ dup key: { : 3 }"
    }
  }
  ["writeConcernError"]=>
  array(2) {
    ["code"]=>
    int(100)
    ["errmsg"]=>
    string(29) "Not enough data-bearing nodes"
  }
  ["writeConcern"]=>
  array(4) {
    ["w"]=>
    int(30)
    ["wmajority"]=>
    bool(false)
    ["wtimeout"]=>
    int(0)
    ["journal"]=>
    NULL
  }
}
===DONE===
