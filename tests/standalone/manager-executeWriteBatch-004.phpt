--TEST--
MongoDB\Manager::executeWriteBatch() with upserted ids
--SKIPIF--
<?php require "tests/utils/basic-skipif.inc" ?>
--FILE--
<?php
require_once "tests/utils/basic.inc";

$manager = new MongoDB\Manager(MONGODB_URI);

$batch = new MongoDB\WriteBatch(false);
$batch->update(array('x' => 'foo'), array('$set' => array('y' => 'foo')), array('upsert' => true));
$batch->update(array('x' => 'bar'), array('$set' => array('y' => 'bar')), array('upsert' => true));
$batch->update(array('x' => 'foo'), array('$set' => array('y' => 'bar')));

$result = $manager->executeWriteBatch(NS, $batch);

echo "\n===> WriteResult\n";
printWriteResult($result);

echo "\n===> Collection\n";
$cursor = $manager->executeQuery(NS, new MongoDB\Query(array()));
var_dump(iterator_to_array($cursor));

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
===> WriteResult
server: localhost:27017
numInserted: 0
numMatched: 1
numModified: 1
numUpserted: 2
numRemoved: 0
upsertedId[0]: object(BSON\ObjectID)#%d (0) {
}
upsertedId[1]: object(BSON\ObjectID)#%d (0) {
}

===> Collection
array(2) {
  [0]=>
  array(3) {
    ["_id"]=>
    object(BSON\ObjectID)#%d (0) {
    }
    ["x"]=>
    string(3) "foo"
    ["y"]=>
    string(3) "bar"
  }
  [1]=>
  array(3) {
    ["_id"]=>
    object(BSON\ObjectID)#%d (0) {
    }
    ["x"]=>
    string(3) "bar"
    ["y"]=>
    string(3) "bar"
  }
}
===DONE===
