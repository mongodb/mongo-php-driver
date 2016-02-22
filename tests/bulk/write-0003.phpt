--TEST--
MongoDB\Driver\BulkWrite: #003 executed multiple times
--SKIPIF--
<?php require __DIR__ . "/../utils/basic-skipif.inc"; CLEANUP(STANDALONE); ?>
--FILE--
<?php
require_once __DIR__ . "/../utils/basic.inc";

$manager = new MongoDB\Driver\Manager(STANDALONE);

$insertBulk = new MongoDB\Driver\BulkWrite;
$insertBulk->insert(['x' => 1]);
$result = $manager->executeBulkWrite(NS, $insertBulk);
printf("Inserted %d document(s)\n", $result->getInsertedCount());

$insertBulk = new MongoDB\Driver\BulkWrite;
$insertBulk->insert(['x' => 1]);
$result = $manager->executeBulkWrite(NS, $insertBulk);
printf("Inserted %d document(s)\n", $result->getInsertedCount());

try {
    $result = $manager->executeBulkWrite(NS, $insertBulk);
    printf("Inserted %d document(s)\n", $result->getInsertedCount());
} catch (MongoDB\Driver\Exception\BulkWriteException $e) {
    printf("Write error: %s\n", $e->getWriteResult()->getWriteErrors()[0]->getMessage());
}

$updateBulk = new MongoDB\Driver\BulkWrite;
$updateBulk->update([], ['$inc' => ['x' => 1]]);
$result = $manager->executeBulkWrite(NS, $updateBulk);
printf("Updated %d document(s)\n", $result->getModifiedCount());

$result = $manager->executeBulkWrite(NS, $updateBulk);
printf("Updated %d document(s)\n", $result->getModifiedCount());

$cursor = $manager->executeQuery(NS, new MongoDB\Driver\Query([]));
var_dump($cursor->toArray());

$deleteBulk = new MongoDB\Driver\BulkWrite;
$deleteBulk->delete([], ['limit' => 1]);
$result = $manager->executeBulkWrite(NS, $deleteBulk);
printf("Deleted %d document(s)\n", $result->getDeletedCount());

$result = $manager->executeBulkWrite(NS, $deleteBulk);
printf("Deleted %d document(s)\n", $result->getDeletedCount());

$cursor = $manager->executeQuery(NS, new MongoDB\Driver\Query([]));
var_dump($cursor->toArray());

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
Inserted 1 document(s)
Inserted 1 document(s)
Write error: E11000 duplicate key error%s
Updated 1 document(s)
Updated 1 document(s)
array(2) {
  [0]=>
  object(stdClass)#%d (%d) {
    ["_id"]=>
    object(MongoDB\BSON\ObjectID)#%d (%d) {
      ["oid"]=>
      string(24) "%x"
    }
    ["x"]=>
    int(3)
  }
  [1]=>
  object(stdClass)#%d (%d) {
    ["_id"]=>
    object(MongoDB\BSON\ObjectID)#%d (%d) {
      ["oid"]=>
      string(24) "%x"
    }
    ["x"]=>
    int(1)
  }
}
Deleted 1 document(s)
Deleted 1 document(s)
array(0) {
}
===DONE===
