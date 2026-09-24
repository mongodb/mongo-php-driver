--TEST--
MongoDB\Driver\BulkWriteCommand::__construct() let
--SKIPIF--
<?php require __DIR__ . "/../utils/basic-skipif.inc"; ?>
<?php skip_if_not_live(); ?>
<?php skip_if_server_version('<', '8.0'); ?>
<?php skip_if_not_clean(); ?>
--FILE--
<?php

require_once __DIR__ . "/../utils/basic.inc";

$manager = create_test_manager();

$let = [
    'targetFlavor' => 'cherry',
    'newFlavor' => 'orange',
];

$bulk = new MongoDB\Driver\BulkWriteCommand(['let' => $let]);

$bulk->insertOne(NS, ['_id' => 1, 'flavor' => 'chocolate']);
$bulk->insertOne(NS, ['_id' => 2, 'flavor' => 'strawberry']);
$bulk->insertOne(NS, ['_id' => 3, 'flavor' => 'cherry']);

$bulk->updateMany(
    NS,
    ['$expr' => ['$eq' => ['$flavor', '$$targetFlavor']]],
    ['$set' => ['flavor' => '$$newFlavor']],
);

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
  int(3)
  ["matchedCount"]=>
  int(1)
  ["modifiedCount"]=>
  int(1)
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
