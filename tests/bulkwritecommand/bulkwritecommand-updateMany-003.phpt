--TEST--
MongoDB\Driver\BulkWriteCommand::updateOne() with pipeline option
--SKIPIF--
<?php require __DIR__ . "/../utils/basic-skipif.inc"; ?>
<?php skip_if_not_live(); ?>
<?php skip_if_server_version('<', '8.0'); ?>
<?php skip_if_not_clean(); ?>
--FILE--
<?php
require_once __DIR__ . "/../utils/basic.inc";

$manager = create_test_manager();

$bulk = new MongoDB\Driver\BulkWriteCommand();

$bulk->insertOne(NS, [ '_id' => 1, 'x' => 1, 'y' => 1, 't' => [ 'u' => [ 'v' => 1 ] ] ]);
$bulk->insertOne(NS, [ '_id' => 2, 'x' => 2, 'y' => 1]);

$manager->executeBulkWriteCommand($bulk);

$updateBulk = new MongoDB\Driver\BulkWriteCommand();

$query = ['_id' => 1];
$update = [
    [
        '$replaceRoot' => [ 'newRoot' => '$t' ],
    ],
    [
        '$addFields' => [ 'foo' => 1 ],
    ],
];

$updateBulk->updateMany(NS, $query, $update);
$manager->executeBulkWriteCommand($updateBulk);

$cursor = $manager->executeQuery(NS, new \MongoDB\Driver\Query([]));
var_dump($cursor->toArray());
?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
array(%d) {
  [0]=>
  object(stdClass)#%d (%d) {
    ["_id"]=>
    int(1)
    ["u"]=>
    object(stdClass)#%d (%d) {
      ["v"]=>
      int(1)
    }
    ["foo"]=>
    int(1)
  }
  [1]=>
  object(stdClass)#%d (%d) {
    ["_id"]=>
    int(2)
    ["x"]=>
    int(2)
    ["y"]=>
    int(1)
  }
}
===DONE===
