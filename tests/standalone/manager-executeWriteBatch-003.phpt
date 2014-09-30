--TEST--
MongoDB\Manager::executeWriteBatch() with duplicate key errors (unordered)
--SKIPIF--
<?php require "tests/utils/basic-skipif.inc" ?>
--FILE--
<?php
require_once "tests/utils/basic.inc";

$manager = new MongoDB\Manager(MONGODB_URI);

$batch = new MongoDB\WriteBatch(false);
$batch->insert(array('_id' => 1));
$batch->insert(array('_id' => 1));
$batch->insert(array('_id' => 2));
$batch->insert(array('_id' => 2));

try {
    $result = $manager->executeWriteBatch(NS, $batch);
    echo "FAILED\n";
} catch (MongoDB\WriteException $e) {
    printf("WriteException.message: %s\n", $e->getMessage());
    printf("WriteException.code: %d\n", $e->getCode());

    echo "\n===> WriteResult\n";
    printWriteResult($e->getWriteResult());
}

echo "\n===> Collection\n";
$cursor = $manager->executeQuery(NS, new MongoDB\Query(array()));
var_dump(iterator_to_array($cursor));

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
WriteException.message: insertDocument :: caused by :: 11000 E11000 duplicate key error index: %s.%s.$_id_  dup key: { : 1 }
WriteException.code: 0

===> WriteResult
server: localhost:27017
numInserted: 2
numMatched: 0
numModified: 0
numUpserted: 0
numRemoved: 0
writeError[1].message: insertDocument :: caused by :: 11000 E11000 duplicate key error index: %s.%s.$_id_  dup key: { : 1 }
writeError[1].code: 11000
writeError[3].message: insertDocument :: caused by :: 11000 E11000 duplicate key error index: %s.%s.$_id_  dup key: { : 2 }
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
