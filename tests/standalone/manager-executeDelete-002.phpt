--TEST--
MongoDB\Manager::executeDelete() multiple documents
--SKIPIF--
<?php require "tests/utils/basic-skipif.inc" ?>
--FILE--
<?php
require_once "tests/utils/basic.inc";

$manager = new MongoDB\Manager(MONGODB_URI);

// load fixtures for test
$manager->executeInsert(NS, array('_id' => 1, 'x' => 1));
$manager->executeInsert(NS, array('_id' => 2, 'x' => 1));

$result = $manager->executeDelete(NS, array('x' => 1), array('limit' => 0));

echo "\n===> WriteResult\n";
printWriteResult($result);

echo "\n===> Collection\n";
$cursor = $manager->executeQuery(NS, new MongoDB\Query(array()));

/* FIXME: Returns NULL, not empty iterator when nothing matches */
if ($cursor) {
    var_dump(iterator_to_array($cursor));
} else {
    var_dump(array());
}

?>
===DONE===
<?php exit(0); ?>
--EXPECT--
===> WriteResult
server: localhost:27017
numInserted: 0
numMatched: 0
numModified: 0
numUpserted: 0
numRemoved: 2

===> Collection
array(0) {
}
===DONE===
