--TEST--
MongoDB\Driver\WriteResult debug output with errors
--SKIPIF--
<?php require __DIR__ . "/../utils/basic-skipif.inc"; ?>
<?php skip_if_not_replica_set(); ?>
<?php skip_if_not_clean(); ?>
--FILE--
<?php
require_once __DIR__ . "/../utils/basic.inc";

$manager = new MongoDB\Driver\Manager(URI);

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
    /* We assume that the replica set does not have 30 nodes */
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
      object(MongoDB\BSON\ObjectId)#%d (%d) {
        ["oid"]=>
        string(24) "%x"
      }
    }
    [1]=>
    array(2) {
      ["index"]=>
      int(1)
      ["_id"]=>
      object(MongoDB\BSON\ObjectId)#%d (%d) {
        ["oid"]=>
        string(24) "%x"
      }
    }
  }
  ["writeErrors"]=>
  array(3) {
    [0]=>
    object(MongoDB\Driver\WriteError)#%d (%d) {
      ["message"]=>
      string(%d) "E11000 duplicate key %S phongo.writeResult_writeresult_debug_002%s dup key: { %S: 1 }"
      ["code"]=>
      int(11000)
      ["index"]=>
      int(3)
      ["info"]=>
      NULL
    }
    [1]=>
    object(MongoDB\Driver\WriteError)#%d (%d) {
      ["message"]=>
      string(%d) "E11000 duplicate key %S phongo.writeResult_writeresult_debug_002%s dup key: { %S: 2 }"
      ["code"]=>
      int(11000)
      ["index"]=>
      int(5)
      ["info"]=>
      NULL
    }
    [2]=>
    object(MongoDB\Driver\WriteError)#%d (%d) {
      ["message"]=>
      string(%d) "E11000 duplicate key %S phongo.writeResult_writeresult_debug_002%s dup key: { %S: 3 }"
      ["code"]=>
      int(11000)
      ["index"]=>
      int(7)
      ["info"]=>
      NULL
    }
  }
  ["writeConcernError"]=>
  object(MongoDB\Driver\WriteConcernError)#%d (%d) {
    ["message"]=>
    string(29) "Not enough data-bearing nodes"
    ["code"]=>
    int(100)
    ["info"]=>
    NULL
  }
  ["writeConcern"]=>
  object(MongoDB\Driver\WriteConcern)#%d (%d) {
    ["w"]=>
    int(30)
  }
}
===DONE===
