--TEST--
MongoDB\Driver\Result::toArray()
--SKIPIF--
<?php require "tests/utils/basic-skipif.inc"; CLEANUP(STANDALONE) ?>
--FILE--
<?php
require_once "tests/utils/basic.inc";

$manager = new MongoDB\Driver\Manager(STANDALONE);

$manager->executeInsert(NS, array('_id' => 1, 'x' => 1));
$manager->executeInsert(NS, array('_id' => 2, 'x' => 1));

$result = $manager->executeQuery(NS, new MongoDB\Driver\Query(array("x" => 1)));

echo "Dumping Result::toArray():\n";
var_dump($result->toArray());

// Execute the query a second time, since we cannot iterate twice
$result = $manager->executeQuery(NS, new MongoDB\Driver\Query(array("x" => 1)));

echo "\nDumping iterated Result:\n";
var_dump(iterator_to_array($result));

?>
===DONE===
<?php exit(0); ?>
--EXPECT--
Dumping Result::toArray():
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

Dumping iterated Result:
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
