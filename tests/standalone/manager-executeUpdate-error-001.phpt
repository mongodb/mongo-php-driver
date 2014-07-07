--TEST--
MongoDB\Manager::executeUpdate() error: multiple documents with upsert
--SKIPIF--
<?php require "tests/utils/basic-skipif.inc" ?>
--FILE--
<?php
require_once "tests/utils/basic.inc";

$manager = new MongoDB\Manager(MONGODB_URI);

try {
    $manager->executeUpdate(
        NS,
        array('_id' => 1),
        array('$set' => array('x' => 1)),
        array('multi' => true, 'upsert' => true)
    );
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
