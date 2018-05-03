--TEST--
MongoDB\Driver\Manager::executeBulkWrite() write concern error
--SKIPIF--
<?php require __DIR__ . "/../utils/basic-skipif.inc"; ?>
<?php skip_if_not_replica_set(); ?>
<?php skip_if_not_clean(); ?>
--FILE--
<?php
require_once __DIR__ . "/../utils/basic.inc";

$manager = new MongoDB\Driver\Manager(URI);

$bulk = new MongoDB\Driver\BulkWrite();
$bulk->insert(array('_id' => 1, 'x' => 1));

try {
    $manager->executeBulkWrite(NS, $bulk, new MongoDB\Driver\WriteConcern(30));
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
BulkWriteException: Not enough data-bearing nodes

===> WriteResult
server: %s:%d
insertedCount: 1
matchedCount: 0
modifiedCount: 0
upsertedCount: 0
deletedCount: 0
object(MongoDB\Driver\WriteConcernError)#%d (%d) {
  ["message"]=>
  string(29) "Not enough data-bearing nodes"
  ["code"]=>
  int(100)
  ["info"]=>
  NULL
}
writeConcernError.message: Not enough data-bearing nodes
writeConcernError.code: 100
writeConcernError.info: NULL

===> Collection
array(1) {
  [0]=>
  object(stdClass)#%d (%d) {
    ["_id"]=>
    int(1)
    ["x"]=>
    int(1)
  }
}
===DONE===
