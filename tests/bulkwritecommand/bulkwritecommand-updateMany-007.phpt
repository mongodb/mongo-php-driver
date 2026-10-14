--TEST--
MongoDB\Driver\BulkWriteCommand::updateMany() PackedArray for arrayFilters option
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

$bulk->insertOne(NS, ['_id' => 1, 'grades' => [95, 92, 90]]);
$bulk->insertOne(NS, ['_id' => 2, 'grades' => [98, 100, 102]]);
$bulk->insertOne(NS, ['_id' => 3, 'grades' => [95, 110, 100]]);

$manager->executeBulkWriteCommand($bulk);

$updateBulk = new MongoDB\Driver\BulkWriteCommand();

$updateBulk->updateMany(NS,
    ['grades' => ['$gte' => 100]],
    ['$set' => ['grades.$[element]' => 100]],
    [
        'arrayFilters' => MongoDB\BSON\PackedArray::fromPHP([['element' => ['$gte' => 100]]]),
        'multi' => true,
    ]
);

$manager->executeBulkWriteCommand($updateBulk);

$cursor = $manager->executeQuery(NS, new MongoDB\Driver\Query([]));

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
    ["grades"]=>
    array(%d) {
      [0]=>
      int(95)
      [1]=>
      int(92)
      [2]=>
      int(90)
    }
  }
  [1]=>
  object(stdClass)#%d (%d) {
    ["_id"]=>
    int(2)
    ["grades"]=>
    array(%d) {
      [0]=>
      int(98)
      [1]=>
      int(100)
      [2]=>
      int(100)
    }
  }
  [2]=>
  object(stdClass)#%d (%d) {
    ["_id"]=>
    int(3)
    ["grades"]=>
    array(%d) {
      [0]=>
      int(95)
      [1]=>
      int(100)
      [2]=>
      int(100)
    }
  }
}
===DONE===
