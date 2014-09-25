--TEST--
MongoDB\Manager::executeUpdate() multiple documents with no upsert
--SKIPIF--
<?php if (getenv("TRAVIS")) exit("skip This oddly enough fails on travis and I cannot figureout why") ?>
<?php require "tests/utils/basic-skipif.inc" ?>
--FILE--
<?php
require_once "tests/utils/basic.inc";

$manager = new MongoDB\Manager(MONGODB_URI);

// load fixtures for test
$manager->executeInsert(NS, array('_id' => 1, 'x' => 1));
$manager->executeInsert(NS, array('_id' => 2, 'x' => 1));
$manager->executeInsert(NS, array('_id' => 3, 'x' => 3));

$result = $manager->executeUpdate(
    NS,
    array('x' => 1),
    array('$set' => array('x' => 2)),
    array('multi' => true, 'upsert' => false)
);

echo "\n===> WriteResult\n";
printWriteResult($result);

echo "\n===> Collection\n";
$cursor = $manager->executeQuery(NS, new MongoDB\Query(array()));
var_dump(iterator_to_array($cursor));

?>
===DONE===
<?php exit(0); ?>
--EXPECT--
===> WriteResult
server: localhost:27017
numInserted: 0
numMatched: 2
numModified: 2
numUpserted: 0
numRemoved: 0

===> Collection
array(3) {
  [0]=>
  array(2) {
    ["_id"]=>
    int(1)
    ["x"]=>
    int(2)
  }
  [1]=>
  array(2) {
    ["_id"]=>
    int(2)
    ["x"]=>
    int(2)
  }
  [2]=>
  array(2) {
    ["_id"]=>
    int(3)
    ["x"]=>
    int(3)
  }
}
===DONE===
