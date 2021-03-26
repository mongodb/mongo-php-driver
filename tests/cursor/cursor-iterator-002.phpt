--TEST--
MongoDB\Driver\Cursor does not allow iterating multiple times (toArray())
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

echo "\nFirst Cursor::toArray():\n";

var_dump($cursor->toArray());

echo "\nSecond Cursor::toArray():\n";

echo throws(function () use ($cursor) {
    var_dump($cursor->toArray());
}, MongoDB\Driver\Exception\LogicException::class), "\n";

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
Inserted: 3

First Cursor::toArray():
array(3) {
  [0]=>
  object(stdClass)#%d (1) {
    ["_id"]=>
    int(0)
  }
  [1]=>
  object(stdClass)#%d (1) {
    ["_id"]=>
    int(1)
  }
  [2]=>
  object(stdClass)#%d (1) {
    ["_id"]=>
    int(2)
  }
}

Second Cursor::toArray():
OK: Got MongoDB\Driver\Exception\LogicException
Cursors cannot rewind after starting iteration
===DONE===
