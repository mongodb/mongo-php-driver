--TEST--
PHPC-898: readConcern option should not be included in getMore commands (query option)
--SKIPIF--
<?php require __DIR__ . "/../utils/basic-skipif.inc"; ?>
<?php skip_if_not_replica_set(); ?>
<?php skip_if_not_clean(); ?>
--FILE--
<?php
require_once __DIR__ . "/../utils/basic.inc";

$manager = create_test_manager();

$bulk = new MongoDB\Driver\BulkWrite();
$bulk->insert(['_id' => 1]);
$bulk->insert(['_id' => 2]);
$bulk->insert(['_id' => 3]);

$result = $manager->executeBulkWrite(NS, $bulk);
printf("Inserted %d document(s)\n", $result->getInsertedCount());

$rc = new MongoDB\Driver\ReadConcern(MongoDB\Driver\ReadConcern::LOCAL);
$query = new MongoDB\Driver\Query([], ['batchSize' => 2, 'readConcern' => $rc]);
$cursor = $manager->executeQuery(NS, $query);

foreach ($cursor as $document) {
    var_dump($document);
}

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
Inserted 3 document(s)
object(stdClass)#%d (1) {
  ["_id"]=>
  int(1)
}
object(stdClass)#%d (1) {
  ["_id"]=>
  int(2)
}
object(stdClass)#%d (1) {
  ["_id"]=>
  int(3)
}
===DONE===
