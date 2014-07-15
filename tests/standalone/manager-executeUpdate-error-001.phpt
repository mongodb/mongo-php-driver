--TEST--
MongoDB\Manager::executeUpdate() error: multiple documents with upsert
--SKIPIF--
<?php exit("skip Why is this supposed to throw LogicException !?") ?>
<?php require "tests/utils/basic-skipif.inc" ?>
--FILE--
<?php
require_once "tests/utils/basic.inc";

$manager = new MongoDB\Manager(MONGODB_URI);

try {
    $result = $manager->executeUpdate(
        NS,
        array('_id' => 1),
        array('$set' => array('x' => 1)),
        array('multi' => true, 'upsert' => true)
    );
printf("Inserted: %d\n", $result->getNumInserted());
printf("Matched: %d\n", $result->getNumMatched());
printf("Modified: %d\n", $result->getNumModified());
printf("Upserted: %d\n", $result->getNumUpserted());
printf("Removed: %d\n", $result->getNumRemoved());
printf("Write concern errors: %d\n", count($result->getWriteConcernErrors()));
printf("Write errors: %d\n", count($result->getWriteErrors()));

    echo "Test failed: expected LogicException was not thrown\n";
} catch (LogicException $e) {
    echo "Expected LogicException was thrown\n";
}

?>
===DONE===
<?php exit(0); ?>
--EXPECT--
Expected LogicException was thrown
===DONE===
