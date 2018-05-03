--TEST--
MongoDB\Driver\WriteResult debug output without errors
--SKIPIF--
<?php require __DIR__ . "/../utils/basic-skipif.inc"; ?>
<?php skip_if_not_live(); ?>
<?php skip_if_not_clean(); ?>
--FILE--
<?php
require_once __DIR__ . "/../utils/basic.inc";

$manager = new MongoDB\Driver\Manager(URI);

$bulk = new MongoDB\Driver\BulkWrite;
$bulk->insert(['x' => 1]);
$bulk->update(['x' => 1], ['$set' => ['y' => 3]]);
$bulk->update(['x' => 2], ['$set' => ['y' => 1]], ['upsert' => true]);
$bulk->update(['x' => 3], ['$set' => ['y' => 2]], ['upsert' => true]);
$bulk->delete(['x' => 1]);

$result = $manager->executeBulkWrite(NS, $bulk);

var_dump($result);

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
object(MongoDB\Driver\WriteResult)#%d (%d) {
  ["nInserted"]=>
  int(1)
  ["nMatched"]=>
  int(1)
  ["nModified"]=>
  int(1)
  ["nRemoved"]=>
  int(1)
  ["nUpserted"]=>
  int(2)
  ["upsertedIds"]=>
  array(2) {
    [0]=>
    array(2) {
      ["index"]=>
      int(2)
      ["_id"]=>
      object(MongoDB\BSON\ObjectId)#%d (%d) {
        ["oid"]=>
        string(24) "%x"
      }
    }
    [1]=>
    array(2) {
      ["index"]=>
      int(3)
      ["_id"]=>
      object(MongoDB\BSON\ObjectId)#%d (%d) {
        ["oid"]=>
        string(24) "%x"
      }
    }
  }
  ["writeErrors"]=>
  array(0) {
  }
  ["writeConcernError"]=>
  NULL
  ["writeConcern"]=>
  object(MongoDB\Driver\WriteConcern)#%d (%d) {
  }
}
===DONE===
