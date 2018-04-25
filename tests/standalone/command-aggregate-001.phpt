--TEST--
DRIVERS-289: Test iteration on live command cursor with empty first batch
--SKIPIF--
<?php require __DIR__ . "/../utils/basic-skipif.inc"; ?>
<?php skip_if_not_live(); ?>
<?php skip_if_not_clean(); ?>
--FILE--
<?php
require_once __DIR__ . "/../utils/basic.inc";

$manager = new MongoDB\Driver\Manager(URI);

$bulk = new MongoDB\Driver\BulkWrite;
$bulk->insert(['_id' => 1]);
$bulk->insert(['_id' => 2]);
$bulk->insert(['_id' => 3]);
$manager->executeBulkWrite(NS, $bulk);

$command = new MongoDB\Driver\Command([
    'aggregate' => COLLECTION_NAME,
    'pipeline' => [
        [ '$match' => [ '_id' => [ '$gt' => 1 ]]],
    ],
    'cursor' => ['batchSize' => 0],
]);

$cursor = $manager->executeCommand(DATABASE_NAME, $command);

var_dump($cursor->toArray());

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
array(2) {
  [0]=>
  object(stdClass)#%d (%d) {
    ["_id"]=>
    int(2)
  }
  [1]=>
  object(stdClass)#%d (%d) {
    ["_id"]=>
    int(3)
  }
}
===DONE===
