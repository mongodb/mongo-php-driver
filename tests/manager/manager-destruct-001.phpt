--TEST--
MongoDB\Driver\Manager destruct should not free streams that are still in use
--SKIPIF--
<?php require __DIR__ . "/../utils/basic-skipif.inc"; ?>
<?php skip_if_not_live(); ?>
<?php skip_if_not_clean(); ?>
--INI--
ignore_repeated_errors=1
--FILE--
<?php
require_once __DIR__ . "/../utils/basic.inc";

$manager1 = new MongoDB\Driver\Manager(URI);
$manager2 = new MongoDB\Driver\Manager(URI);

$bulk = new MongoDB\Driver\BulkWrite();
$bulk->insert(array('_id' => 1));
$writeResult = $manager1->executeBulkWrite(NS, $bulk);
printf("Inserted: %d\n", $writeResult->getInsertedCount());

$bulk = new MongoDB\Driver\BulkWrite();
$bulk->insert(array('_id' => 2));
$writeResult = $manager2->executeBulkWrite(NS, $bulk);
printf("Inserted: %d\n", $writeResult->getInsertedCount());

$manager2 = null;

$bulk = new MongoDB\Driver\BulkWrite();
$bulk->insert(array('_id' => 3));
$writeResult = $manager1->executeBulkWrite(NS, $bulk);
printf("Inserted: %d\n", $writeResult->getInsertedCount());

?>
===DONE===
<?php exit(0); ?>
--EXPECT--
Inserted: 1
Inserted: 1
Inserted: 1
===DONE===
