--TEST--
MongoDB\Driver\BulkWriteCommand::__construct() verboseResults=false
--SKIPIF--
<?php require __DIR__ . "/../utils/basic-skipif.inc"; ?>
<?php skip_if_not_live(); ?>
<?php skip_if_server_version('<', '8.0'); ?>
<?php skip_if_not_clean(); ?>
--FILE--
<?php

require_once __DIR__ . "/../utils/basic.inc";

$manager = create_test_manager();

$bulk = new MongoDB\Driver\BulkWriteCommand(['verboseResults' => false]);
$bulk->insertOne(NS, ['_id' => 1]);
$bulk->updateOne(NS, ['_id' => 1], ['$set' => ['x' => 1]]);
$bulk->deleteOne(NS, ['_id' => 1]);

$result = $manager->executeBulkWriteCommand($bulk);

var_dump($result);

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
object(MongoDB\Driver\BulkWriteCommandResult)#%d (%d) {
  ["isAcknowledged"]=>
  bool(true)
  ["insertedCount"]=>
  int(1)
  ["matchedCount"]=>
  int(1)
  ["modifiedCount"]=>
  int(1)
  ["upsertedCount"]=>
  int(0)
  ["deletedCount"]=>
  int(1)
  ["insertResults"]=>
  NULL
  ["updateResults"]=>
  NULL
  ["deleteResults"]=>
  NULL
}
===DONE===
