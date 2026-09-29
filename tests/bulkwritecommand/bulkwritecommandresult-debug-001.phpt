--TEST--
MongoDB\Driver\BulkWriteCommandResult debug output with unacknowledged write concern
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

var_dump($result);

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
object(MongoDB\Driver\BulkWriteCommandResult)#%d (%d) {
  ["isAcknowledged"]=>
  bool(false)
  ["insertedCount"]=>
  int(0)
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
===DONE===
