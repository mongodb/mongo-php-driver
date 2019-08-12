--TEST--
MongoDB\Driver\BulkWrite::update() with pipeline option
--SKIPIF--
<?php require __DIR__ . "/../utils/basic-skipif.inc"; ?>
<?php skip_if_not_live(); ?>
<?php skip_if_server_version('<', '4.2'); ?>
<?php skip_if_not_clean(); ?>
--FILE--
<?php
require_once __DIR__ . "/../utils/basic.inc";

$manager = new MongoDB\Driver\Manager(URI);

$bulk = new MongoDB\Driver\BulkWrite();

$bulk->insert([ '_id' => 1, 'x' => 1, 'y' => 1, 't' => [ 'u' => [ 'v' => 1 ] ] ]);
$bulk->insert([ '_id' => 2, 'x' => 2, 'y' => 1]);

$manager->executeBulkWrite(NS, $bulk);

$updateBulk = new MongoDB\Driver\BulkWrite();

$query = ['_id' => 1];
$update = [
    [
        '$replaceRoot' => [ 'newRoot' => '$t' ],
    ],
    [
        '$addFields' => [ 'foo' => 1 ],
    ],
];

$updateBulk->update($query, $update);
$manager->executeBulkWrite(NS, $updateBulk);

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
