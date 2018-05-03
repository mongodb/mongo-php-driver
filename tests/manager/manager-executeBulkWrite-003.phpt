--TEST--
MongoDB\Driver\Manager::executeBulkWrite() delete one document
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
$manager->executeBulkWrite(NS, $bulk);

$bulk = new MongoDB\Driver\BulkWrite();
$bulk->delete(array('x' => 1), array('limit' => 1));
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
matchedCount: 0
modifiedCount: 0
upsertedCount: 0
deletedCount: 1

===> Collection
array(1) {
  [0]=>
  object(stdClass)#%d (2) {
    ["_id"]=>
    int(2)
    ["x"]=>
    int(1)
  }
}
===DONE===
