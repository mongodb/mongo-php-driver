--TEST--
MongoDB\Driver\Manager::executeWriteBatch() with duplicate key errors (unordered)
--SKIPIF--
<?php require "tests/utils/basic-skipif.inc" ?>
--FILE--
<?php
require_once "tests/utils/basic.inc";

$manager = new MongoDB\Driver\Manager(MONGODB_URI);

$batch = new MongoDB\Driver\WriteBatch(false);
$batch->insert(array('_id' => 1));
$batch->insert(array('_id' => 1));
$batch->insert(array('_id' => 2));
$batch->insert(array('_id' => 2));

try {
    $result = $manager->executeWriteBatch(NS, $batch);
    echo "FAILED\n";
} catch (MongoDB\Driver\WriteException $e) {
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
WriteException.message: %s
WriteException.code: 11000

===> WriteResult
server: %s:%d
insertedCount: 2
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
}
writeError[1].message: %s
writeError[1].code: 11000
object(MongoDB\Driver\WriteError)#%d (%d) {
  ["message"]=>
  string(%d) "%s"
  ["code"]=>
  int(11000)
  ["index"]=>
  int(3)
}
writeError[3].message: %s
writeError[3].code: 11000

===> Collection
array(2) {
  [0]=>
  array(1) {
    ["_id"]=>
    int(1)
  }
  [1]=>
  array(1) {
    ["_id"]=>
    int(2)
  }
}
===DONE===
