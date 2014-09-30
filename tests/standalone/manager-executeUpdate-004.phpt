--TEST--
MongoDB\Manager::executeUpdate() multiple documents with upsert
--SKIPIF--
<?php require "tests/utils/basic-skipif.inc" ?>
--FILE--
<?php
require_once "tests/utils/basic.inc";

$manager = new MongoDB\Manager(MONGODB_URI);

$result = $manager->executeUpdate(
    NS,
    array('_id' => 1),
    array('$set' => array('x' => 1)),
    array('multi' => true, 'upsert' => true)
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
numMatched: 0
numModified: 0
numUpserted: 1
numRemoved: 0
upsertedId[0]: int(1)

===> Collection
array(1) {
  [0]=>
  array(2) {
    ["_id"]=>
    int(1)
    ["x"]=>
    int(1)
  }
}
===DONE===
