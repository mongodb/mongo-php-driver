--TEST--
MongoDB\Manager::executeUpdate() one document with no upsert
--SKIPIF--
<?php require "tests/utils/basic-skipif.inc" ?>
--FILE--
<?php
require_once "tests/utils/basic.inc";

$manager = new MongoDB\Manager(MONGODB_URI);

// load fixtures for test
$manager->executeInsert(NS, array('_id' => 1, 'x' => 1));

$result = $manager->executeUpdate(
    NS,
    array('_id' => 1),
    array('$set' => array('x' => 2)),
    array('multi' => false, 'upsert' => false)
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
numMatched: 1
numModified: 1
numUpserted: 0
numRemoved: 0

===> Collection
array(1) {
  [0]=>
  array(2) {
    ["_id"]=>
    int(1)
    ["x"]=>
    int(2)
  }
}
===DONE===
