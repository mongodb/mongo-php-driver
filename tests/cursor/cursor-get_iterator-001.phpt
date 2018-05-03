--TEST--
MongoDB\Driver\Cursor get_iterator handler does not yield multiple iterators (foreach)
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

echo "\nFirst foreach statement:\n";

foreach ($cursor as $document) {
    var_dump($document);
}

echo "\nSecond foreach statement:\n";

try {
    foreach ($cursor as $document) {
        echo "FAILED: get_iterator should not yield multiple iterators\n";
    }
} catch (MongoDB\Driver\Exception\LogicException $e) {
    printf("LogicException: %s\n", $e->getMessage());
}

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
Inserted: 3

First foreach statement:
object(stdClass)#%d (1) {
  ["_id"]=>
  int(0)
}
object(stdClass)#%d (1) {
  ["_id"]=>
  int(1)
}
object(stdClass)#%d (1) {
  ["_id"]=>
  int(2)
}

Second foreach statement:
LogicException: Cursors cannot yield multiple iterators
===DONE===
