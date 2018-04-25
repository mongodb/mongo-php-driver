--TEST--
MongoDB\Driver\Cursor get_iterator handler does not yield multiple iterators (toArray())
--SKIPIF--
<?php require __DIR__ . "/../utils/basic-skipif.inc"; ?>
<?php skip_if_not_live(); ?>
<?php skip_if_not_clean(); ?>
--FILE--
<?php
require_once __DIR__ . "/../utils/basic.inc";

$manager = new MongoDB\Driver\Manager(URI);

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

try {
    var_dump($cursor->toArray());
} catch (MongoDB\Driver\Exception\LogicException $e) {
    printf("LogicException: %s\n", $e->getMessage());
}

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
LogicException: Cursors cannot yield multiple iterators
===DONE===
