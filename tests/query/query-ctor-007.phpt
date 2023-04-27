--TEST--
MongoDB\Driver\Query::__construct() $filter is MongoDB\Driver\Document
--SKIPIF--
<?php require __DIR__ . "/../utils/basic-skipif.inc"; ?>
<?php skip_if_not_live(); ?>
<?php skip_if_not_clean(); ?>
--FILE--
<?php

require_once __DIR__ . "/../utils/basic.inc";

$manager = create_test_manager();

$bulk = new MongoDB\Driver\BulkWrite;
$bulk->insert(['_id' => 1]);
$bulk->insert(['_id' => 2]);
$manager->executeBulkWrite(NS, $bulk);

$filter = MongoDB\BSON\Document::fromJSON('{ "_id": { "$gt": 1 } }');
$query = new MongoDB\Driver\Query($filter);

var_dump($query);

$cursor = $manager->executeQuery(NS, $query);

var_dump($cursor->toArray());

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
object(MongoDB\Driver\Query)#%d (%d) {
  ["filter"]=>
  object(stdClass)#%d (%d) {
    ["_id"]=>
    object(stdClass)#%d (%d) {
      ["$gt"]=>
      int(1)
    }
  }
  ["options"]=>
  object(stdClass)#%d (%d) {
  }
  ["readConcern"]=>
  NULL
}
array(1) {
  [0]=>
  object(stdClass)#%d (%d) {
    ["_id"]=>
    int(2)
  }
}
===DONE===
