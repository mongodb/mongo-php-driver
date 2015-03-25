--TEST--
MongoDB\Driver\Manager::executeUpdate() one document with no upsert
--SKIPIF--
<?php require __DIR__ . "/../utils/basic-skipif.inc"; CLEANUP(STANDALONE) ?>
--FILE--
<?php
require_once __DIR__ . "/../utils/basic.inc";

$manager = new MongoDB\Driver\Manager(STANDALONE);

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
$cursor = $manager->executeQuery(NS, new MongoDB\Driver\Query(array()));
var_dump(iterator_to_array($cursor));

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
===> WriteResult
server: %s:%d
insertedCount: 0
matchedCount: 1
modifiedCount: 1
upsertedCount: 0
deletedCount: 0

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
