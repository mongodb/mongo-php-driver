--TEST--
MongoDB\Driver\Cursor allows exposing raw BSON objects
--SKIPIF--
<?php require __DIR__ . "/../utils/basic-skipif.inc"; ?>
<?php skip_if_not_live(); ?>
<?php skip_if_not_clean(); ?>
--FILE--
<?php
require_once __DIR__ . "/../utils/basic.inc";

$manager = create_test_manager();

$bulkWrite = new MongoDB\Driver\BulkWrite;

for ($i = 0; $i < 3; $i++) {
    $bulkWrite->insert(array('_id' => $i));
}

$writeResult = $manager->executeBulkWrite(NS, $bulkWrite);
printf("Inserted: %d\n", $writeResult->getInsertedCount());

$cursor = $manager->executeQuery(NS, new MongoDB\Driver\Query(array()));
$cursor->setTypemap(['root' => 'bson']);

echo "\nFirst foreach statement:\n";

foreach ($cursor as $document) {
    var_dump($document);
}

echo "\nSecond foreach statement:\n";

echo throws(function () use ($cursor) {
    foreach ($cursor as $document) {
        echo "FAILED: get_iterator should not yield multiple iterators\n";
    }
}, MongoDB\Driver\Exception\LogicException::class), "\n";

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
Inserted: 3

First foreach statement:
object(MongoDB\BSON\Document)#%d (%d) {
  ["data"]=>
  string(20) "DgAAABBfaWQAAAAAAAA="
  ["value"]=>
  object(stdClass)#%d (%d) {
    ["_id"]=>
    int(0)
  }
}
object(MongoDB\BSON\Document)#%d (%d) {
  ["data"]=>
  string(20) "DgAAABBfaWQAAQAAAAA="
  ["value"]=>
  object(stdClass)#%d (%d) {
    ["_id"]=>
    int(1)
  }
}
object(MongoDB\BSON\Document)#%d (%d) {
  ["data"]=>
  string(20) "DgAAABBfaWQAAgAAAAA="
  ["value"]=>
  object(stdClass)#%d (%d) {
    ["_id"]=>
    int(2)
  }
}

Second foreach statement:
OK: Got MongoDB\Driver\Exception\LogicException
Cursors cannot rewind after starting iteration
===DONE===
