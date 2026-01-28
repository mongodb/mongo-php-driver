--TEST--
MongoDB\Driver\BulkWriteCommand::__construct() ordered=true
--SKIPIF--
<?php require __DIR__ . "/../utils/basic-skipif.inc"; ?>
<?php skip_if_not_live(); ?>
<?php skip_if_server_version('<', '8.0'); ?>
<?php skip_if_not_clean(); ?>
--FILE--
<?php

require_once __DIR__ . "/../utils/basic.inc";

$manager = create_test_manager();

$bulk = new MongoDB\Driver\BulkWriteCommand(['ordered' => true]);
$bulk->insertOne(NS, ['_id' => 1]);
$bulk->insertOne(NS, ['_id' => 1]);
$bulk->insertOne(NS, ['_id' => 2]);

try {
    $manager->executeBulkWriteCommand($bulk);
} catch (MongoDB\Driver\Exception\BulkWriteCommandException $e) {
    printf("%s(%d): %s\n", get_class($e), $e->getCode(), $e->getMessage());
    var_dump($e->getPartialResult());
    var_dump($e->getWriteErrors());
}

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
MongoDB\Driver\Exception\BulkWriteCommandException(0): Bulk write failed
object(MongoDB\Driver\BulkWriteCommandResult)#%d (%d) {
  ["isAcknowledged"]=>
  bool(true)
  ["insertedCount"]=>
  int(1)
  ["matchedCount"]=>
  int(0)
  ["modifiedCount"]=>
  int(0)
  ["upsertedCount"]=>
  int(0)
  ["deletedCount"]=>
  int(0)
  ["insertResults"]=>
  NULL
  ["updateResults"]=>
  NULL
  ["deleteResults"]=>
  NULL
}
array(1) {
  [1]=>
  object(MongoDB\Driver\WriteError)#%d (%d) {
    ["message"]=>
    string(128) "E11000 duplicate key error collection: phongo.bulkwritecommand_bulkwritecommand_ctor_ordered_001 index: _id_ dup key: { _id: 1 }"
    ["code"]=>
    int(11000)
    ["index"]=>
    int(1)
    ["info"]=>
    object(stdClass)#%d (%d) {
    }
  }
}
===DONE===
