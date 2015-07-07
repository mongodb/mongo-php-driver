--TEST--
MongoDB\Driver\Manager::executeBulkWrite() with duplicate key errors (ordered)
--SKIPIF--
<?php require __DIR__ . "/../utils/basic-skipif.inc"; CLEANUP(STANDALONE) ?>
--FILE--
<?php
require_once __DIR__ . "/../utils/basic.inc";

$manager = new MongoDB\Driver\Manager(STANDALONE);

$bulk = new MongoDB\Driver\BulkWrite(true);
$bulk->insert(array('_id' => 1));
$bulk->insert(array('_id' => 1));
$bulk->insert(array('_id' => 2));
$bulk->insert(array('_id' => 2));

try {
    $result = $manager->executeBulkWrite(NS, $bulk);
    echo "FAILED\n";
} catch (MongoDB\Driver\Exception\WriteException $e) {
    printf("WriteException.message: %s\n", $e->getMessage());
    printf("WriteException.code: %d\n", $e->getCode());

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
WriteException.message: BulkWrite error
WriteException.code: 0

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
