--TEST--
MongoDB\Driver\Cursor::toArray()
--SKIPIF--
<?php require __DIR__ . "/../utils/basic-skipif.inc"; CLEANUP(STANDALONE) ?>
--FILE--
<?php
require_once __DIR__ . "/../utils/basic.inc";

$manager = new MongoDB\Driver\Manager(STANDALONE);

$manager->executeInsert(NS, array('_id' => 1, 'x' => 1));
$manager->executeInsert(NS, array('_id' => 2, 'x' => 1));

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
--EXPECT--
Dumping Cursor::toArray():
array(2) {
  [0]=>
  array(2) {
    ["_id"]=>
    int(1)
    ["x"]=>
    int(1)
  }
  [1]=>
  array(2) {
    ["_id"]=>
    int(2)
    ["x"]=>
    int(1)
  }
}

Dumping iterated Cursor:
array(2) {
  [0]=>
  array(2) {
    ["_id"]=>
    int(1)
    ["x"]=>
    int(1)
  }
  [1]=>
  array(2) {
    ["_id"]=>
    int(2)
    ["x"]=>
    int(1)
  }
}
===DONE===
