--TEST--
MongoDB\Driver\BulkWrite::update() PackedArray for update pipeline
--SKIPIF--
<?php require __DIR__ . "/../utils/basic-skipif.inc"; ?>
<?php skip_if_not_live(); ?>
<?php skip_if_server_version('<', '4.2'); ?>
<?php skip_if_not_clean(); ?>
--FILE--
<?php
require_once __DIR__ . "/../utils/basic.inc";

$manager = create_test_manager();

$bulk = new MongoDB\Driver\BulkWrite();

$bulk->insert(['_id' => 1, 'x' => 1, 'y' => 1, 't' => ['u' => ['v' => 1]]]);
$bulk->insert(['_id' => 2, 'x' => 2, 'y' => 1]);

$manager->executeBulkWrite(NS, $bulk);

$updateBulk = new MongoDB\Driver\BulkWrite();

$updateBulk->update(
    ['_id' => 1],
    MongoDB\BSON\PackedArray::fromPHP([
        ['$replaceRoot' => ['newRoot' => '$t']],
        ['$addFields' => ['foo' => 1]],
    ])
);

$manager->executeBulkWrite(NS, $updateBulk);

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
