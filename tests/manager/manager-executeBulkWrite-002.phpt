--TEST--
MongoDB\Driver\Manager::executeBulkWrite() with upserted ids
--SKIPIF--
<?php require __DIR__ . "/../utils/basic-skipif.inc"; ?>
<?php skip_if_not_live(); ?>
<?php skip_if_not_clean(); ?>
--FILE--
<?php
require_once __DIR__ . "/../utils/basic.inc";

$manager = new MongoDB\Driver\Manager(URI);

$bulk = new MongoDB\Driver\BulkWrite(['ordered' => false]);
$bulk->update(array('x' => 'foo'), array('$set' => array('y' => 'foo')), array('upsert' => true));
$bulk->update(array('x' => 'bar'), array('$set' => array('y' => 'bar')), array('upsert' => true));
$bulk->update(array('x' => 'foo'), array('$set' => array('y' => 'bar')));

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
matchedCount: 1
modifiedCount: 1
upsertedCount: 2
deletedCount: 0
upsertedId[0]: object(%s\ObjectId)#%d (%d) {
  ["oid"]=>
  string(24) "%s"
}
upsertedId[1]: object(%s\ObjectId)#%d (%d) {
  ["oid"]=>
  string(24) "%s"
}

===> Collection
array(2) {
  [0]=>
  object(stdClass)#%d (3) {
    ["_id"]=>
    object(%s\ObjectId)#%d (%d) {
      ["oid"]=>
      string(24) "%s"
    }
    ["x"]=>
    string(3) "foo"
    ["y"]=>
    string(3) "bar"
  }
  [1]=>
  object(stdClass)#%d (3) {
    ["_id"]=>
    object(%s\ObjectId)#%d (%d) {
      ["oid"]=>
      string(24) "%s"
    }
    ["x"]=>
    string(3) "bar"
    ["y"]=>
    string(3) "bar"
  }
}
===DONE===
