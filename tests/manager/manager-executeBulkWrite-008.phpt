--TEST--
MongoDB\Driver\Manager::executeBulkWrite() update multiple documents with no upsert
--SKIPIF--
<?php require __DIR__ . "/../utils/basic-skipif.inc"; ?>
<?php skip_if_not_live(); ?>
<?php skip_if_not_clean(); ?>
--FILE--
<?php
require_once __DIR__ . "/../utils/basic.inc";

$manager = new MongoDB\Driver\Manager(URI);

// load fixtures for test
$bulk = new MongoDB\Driver\BulkWrite();
$bulk->insert(array('_id' => 1, 'x' => 1));
$bulk->insert(array('_id' => 2, 'x' => 1));
$bulk->insert(array('_id' => 3, 'x' => 3));
$manager->executeBulkWrite(NS, $bulk);

$bulk = new MongoDB\Driver\BulkWrite();
$bulk->update(
    array('x' => 1),
    array('$set' => array('x' => 2)),
    array('multi' => true, 'upsert' => false)
);
$result = $manager->executeBulkWrite(NS, $bulk);

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
matchedCount: 2
modifiedCount: 2
upsertedCount: 0
deletedCount: 0

===> Collection
array(3) {
  [0]=>
  object(stdClass)#%d (2) {
    ["_id"]=>
    int(1)
    ["x"]=>
    int(2)
  }
  [1]=>
  object(stdClass)#%d (2) {
    ["_id"]=>
    int(2)
    ["x"]=>
    int(2)
  }
  [2]=>
  object(stdClass)#%d (2) {
    ["_id"]=>
    int(3)
    ["x"]=>
    int(3)
  }
}
===DONE===
