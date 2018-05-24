--TEST--
MongoDB\Driver\Command with update and arrayFilters
--SKIPIF--
<?php require __DIR__ . "/../utils/basic-skipif.inc"; ?>
<?php skip_if_not_live(); ?>
<?php skip_if_server_version('<', '3.6'); ?>
<?php skip_if_not_clean(); ?>
--FILE--
<?php
require_once __DIR__ . "/../utils/basic.inc";

$manager = new MongoDB\Driver\Manager(URI);

$bulk = new MongoDB\Driver\BulkWrite();

$bulk->insert([ '_id' => 1, 'grades' => [ 95, 92, 90 ] ]);
$bulk->insert([ '_id' => 2, 'grades' => [ 98, 100, 102 ] ]);
$bulk->insert([ '_id' => 3, 'grades' => [ 95, 110, 100 ] ]);

$manager->executeBulkWrite(DATABASE_NAME . '.' . COLLECTION_NAME, $bulk);

$command = new MongoDB\Driver\Command([
    'update' => COLLECTION_NAME, 
    'updates' => [[
        'q' => [ 'grades' => [ '$gte' => 100 ] ],
        'u' => [ '$set' => [ 'grades.$[element]' => 100 ] ],
        'arrayFilters' => [ [ 'element' => [ '$gte' => 100 ] ] ],
        'multi' => true
    ]],
]);

$manager->executeCommand(DATABASE_NAME, $command);

$cursor = $manager->executeQuery( DATABASE_NAME . '.' . COLLECTION_NAME, new \MongoDB\Driver\Query([]));
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
