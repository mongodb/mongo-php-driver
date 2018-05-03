--TEST--
MongoDB\Driver\Cursor get_iterator handler does not yield multiple iterators (IteratorIterator)
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

echo "\nFirst IteratorIterator wrapping:\n";

var_dump(new IteratorIterator($cursor));

echo "\nSecond IteratorIterator wrapping:\n";

try {
    var_dump(new IteratorIterator($cursor));
} catch (MongoDB\Driver\Exception\LogicException $e) {
    printf("LogicException: %s\n", $e->getMessage());
}

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
Inserted: 3

First IteratorIterator wrapping:
object(IteratorIterator)#%d (0) {
}

Second IteratorIterator wrapping:
LogicException: Cursors cannot yield multiple iterators
===DONE===
