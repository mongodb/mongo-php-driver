--TEST--
MongoDB\Driver\BulkWrite::update with arrayFilters
--SKIPIF--
<?php require __DIR__ . "/../utils/basic-skipif.inc"; ?>
<?php START('THROWAWAY', ["version" => "36-release"]); CLEANUP(THROWAWAY); ?>
--FILE--
<?php
require_once __DIR__ . "/../utils/basic.inc";

$manager = new MongoDB\Driver\Manager(THROWAWAY);

$bulk = new MongoDB\Driver\BulkWrite();

$bulk->insert([ '_id' => 1, 'grades' => [ 95, 92, 90 ] ]);
$bulk->insert([ '_id' => 2, 'grades' => [ 98, 100, 102 ] ]);
$bulk->insert([ '_id' => 3, 'grades' => [ 95, 110, 100 ] ]);

$manager->executeBulkWrite(DATABASE_NAME . '.' . COLLECTION_NAME, $bulk);

$updateBulk = new MongoDB\Driver\BulkWrite();

$query = new MongoDB\Driver\Query(['grades' => ['$gte' => 100]]);
$update = [ '$set' => [ 'grades.$[element]' => 100 ] ];
$options = [
    'arrayFilters' => [ [ 'element' => [ '$gte' => 100 ] ] ],
    'multi' => true
];

$updateBulk->update($query, $update, $options);
$manager->executeBulkWrite(DATABASE_NAME . '.' . COLLECTION_NAME, $updateBulk);

$cursor = $manager->executeQuery( DATABASE_NAME . '.' . COLLECTION_NAME, new \MongoDB\Driver\Query([]));
var_dump($cursor->toArray());
?>
===DONE===
<?php DELETE("THROWAWAY"); ?>
<?php exit(0); ?>
--CLEAN--
<?php require __DIR__ . "/../utils/basic-skipif.inc"; ?>
<?php DELETE("THROWAWAY"); ?>
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
