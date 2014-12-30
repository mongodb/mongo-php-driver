--TEST--
MongoDB\Driver\Manager::executeWriteBatch()
--SKIPIF--
<?php require "tests/utils/basic-skipif.inc" ?>
--FILE--
<?php
require_once "tests/utils/basic.inc";

$manager = new MongoDB\Driver\Manager(MONGODB_URI);

$batch = new MongoDB\Driver\WriteBatch();
$batch->insert(array('_id' => 1, 'x' => 1));
$batch->insert(array('_id' => 2, 'x' => 2));
$batch->update(array('x' => 2), array('$set' => array('x' => 1)), array("limit" => 1, "upsert" => false));
$batch->update(array('_id' => 3), array('$set' => array('x' => 3)), array("limit" => 1, "upsert" => true));
$batch->delete(array('x' => 1), array("limit" => 1));

$result = $manager->executeWriteBatch(NS, $batch);

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
insertedCount: 2
matchedCount: 1
modifiedCount: 1
upsertedCount: 1
deletedCount: 1
upsertedId[3]: int(3)

===> Collection
array(2) {
  [0]=>
  array(2) {
    ["_id"]=>
    int(2)
    ["x"]=>
    int(1)
  }
  [1]=>
  array(2) {
    ["_id"]=>
    int(3)
    ["x"]=>
    int(3)
  }
}
===DONE===
