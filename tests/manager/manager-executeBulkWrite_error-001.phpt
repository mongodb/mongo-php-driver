--TEST--
MongoDB\Driver\Manager::executeBulkWrite() with duplicate key errors (ordered)
--SKIPIF--
<?php require __DIR__ . "/../utils/basic-skipif.inc"; ?>
<?php skip_if_not_live(); ?>
<?php skip_if_not_clean(); ?>
--FILE--
<?php
require_once __DIR__ . "/../utils/basic.inc";

$manager = new MongoDB\Driver\Manager(URI);

$bulk = new MongoDB\Driver\BulkWrite(['ordered' => true]);
$bulk->insert(array('_id' => 1));
$bulk->insert(array('_id' => 1));
$bulk->insert(array('_id' => 2));
$bulk->insert(array('_id' => 2));

try {
    $result = $manager->executeBulkWrite(NS, $bulk);
    echo "FAILED\n";
} catch (MongoDB\Driver\Exception\BulkWriteException $e) {
    printf("BulkWriteException: %s\n", $e->getMessage());

    echo "\n===> WriteResult\n";
    printWriteResult($e->getWriteResult());
}

echo "\n===> Collection\n";
$cursor = $manager->executeQuery(NS, new MongoDB\Driver\Query(array()));
var_dump(iterator_to_array($cursor));

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
BulkWriteException:%S E11000 duplicate key error %s: phongo.manager_manager_executeBulkWrite_error_001%sdup key: { %S: 1 }

===> WriteResult
server: %s:%d
insertedCount: 1
matchedCount: 0
modifiedCount: 0
upsertedCount: 0
deletedCount: 0
object(MongoDB\Driver\WriteError)#%d (%d) {
  ["message"]=>
  string(%d) "%s"
  ["code"]=>
  int(11000)
  ["index"]=>
  int(1)
  ["info"]=>
  NULL
}
writeError[1].message: %s
writeError[1].code: 11000

===> Collection
array(1) {
  [0]=>
  object(stdClass)#%d (1) {
    ["_id"]=>
    int(1)
  }
}
===DONE===
