--TEST--
MongoDB\Driver\Cursor::toArray()
--SKIPIF--
<?php require __DIR__ . "/../utils/basic-skipif.inc"; ?>
<?php skip_if_not_live(); ?>
<?php skip_if_not_clean(); ?>
--FILE--
<?php
require_once __DIR__ . "/../utils/basic.inc";

$manager = new MongoDB\Driver\Manager(URI);

$bulk = new MongoDB\Driver\BulkWrite();
$bulk->insert(array('_id' => 1, 'x' => 1));
$bulk->insert(array('_id' => 2, 'x' => 1));
$manager->executeBulkWrite(NS, $bulk);

$cursor = $manager->executeQuery(NS, new MongoDB\Driver\Query(array("x" => 1)));

echo "Dumping Cursor::toArray():\n";
var_dump($cursor->toArray());

// Execute the query a second time, since we cannot iterate twice
$cursor = $manager->executeQuery(NS, new MongoDB\Driver\Query(array("x" => 1)));

echo "\nDumping iterated Cursor:\n";
var_dump(iterator_to_array($cursor));

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
Dumping Cursor::toArray():
array(2) {
  [0]=>
  object(stdClass)#%d (2) {
    ["_id"]=>
    int(1)
    ["x"]=>
    int(1)
  }
  [1]=>
  object(stdClass)#%d (2) {
    ["_id"]=>
    int(2)
    ["x"]=>
    int(1)
  }
}

Dumping iterated Cursor:
array(2) {
  [0]=>
  object(stdClass)#%d (2) {
    ["_id"]=>
    int(1)
    ["x"]=>
    int(1)
  }
  [1]=>
  object(stdClass)#%d (2) {
    ["_id"]=>
    int(2)
    ["x"]=>
    int(1)
  }
}
===DONE===
