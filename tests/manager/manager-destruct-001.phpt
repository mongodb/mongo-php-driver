--TEST--
MongoDB\Driver\Manager destruct should not free streams that are still in use
--SKIPIF--
<?php require __DIR__ . "/../utils/basic-skipif.inc"; CLEANUP(STANDALONE) ?>
--INI--
ignore_repeated_errors=1
--FILE--
<?php
require_once __DIR__ . "/../utils/basic.inc";

$manager1 = new MongoDB\Driver\Manager(STANDALONE);
$manager2 = new MongoDB\Driver\Manager(STANDALONE);

$writeResult = $manager1->executeInsert(NS, array('_id' => 1));
printf("Inserted: %d\n", $writeResult->getInsertedCount());

$writeResult = $manager2->executeInsert(NS, array('_id' => 2));
printf("Inserted: %d\n", $writeResult->getInsertedCount());

$manager2 = null;

$writeResult = $manager1->executeInsert(NS, array('_id' => 3));
printf("Inserted: %d\n", $writeResult->getInsertedCount());

?>
===DONE===
<?php exit(0); ?>
--EXPECT--
Inserted: 1
Inserted: 1
Inserted: 1
===DONE===
