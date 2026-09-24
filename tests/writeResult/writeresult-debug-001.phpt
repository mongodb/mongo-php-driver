--TEST--
MongoDB\Driver\WriteResult debug output without errors
--SKIPIF--
<?php require __DIR__ . "/../utils/basic-skipif.inc"; ?>
<?php skip_if_not_live(); ?>
<?php skip_if_not_clean(); ?>
--FILE--
<?php
require_once __DIR__ . "/../utils/basic.inc";

$manager = create_test_manager();

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
  ["insertedCount"]=>
  int(1)
  ["matchedCount"]=>
  int(1)
  ["modifiedCount"]=>
  int(1)
  ["deletedCount"]=>
  int(1)
  ["upsertedCount"]=>
  int(2)
  ["server"]=>
  object(MongoDB\Driver\Server)#%d (%d) {
    %a
  }
  ["upsertedIds"]=>
  array(2) {
    [2]=>
    object(MongoDB\BSON\ObjectId)#%d (%d) {
      ["oid"]=>
      string(24) "%x"
    }
    [3]=>
    object(MongoDB\BSON\ObjectId)#%d (%d) {
      ["oid"]=>
      string(24) "%x"
    }
  }
  ["writeErrors"]=>
  array(0) {
  }
  ["writeConcernError"]=>
  NULL
  ["writeConcern"]=>
  object(MongoDB\Driver\WriteConcern)#%d (3) {
    ["w"]=>
    NULL
    ["j"]=>
    NULL
    ["wtimeout"]=>
    int(0)
  }
  ["errorReplies"]=>
  array(0) {
  }
}
===DONE===
