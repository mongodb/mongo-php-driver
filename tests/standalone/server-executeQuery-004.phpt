--TEST--
MongoDB\Driver\Manager::executeDelete() multiple documents
--SKIPIF--
<?php require "tests/utils/basic-skipif.inc" ?>
--FILE--
<?php
require_once "tests/utils/basic.inc";

$manager = new MongoDB\Driver\Manager(MONGODB_URI);

$manager->executeInsert(NS, array('_id' => 1, 'x' => 1));
$manager->executeInsert(NS, array('_id' => 2, 'x' => 1));

$cursor = $manager->executeQuery(NS, new MongoDB\Driver\Query(array("x" => 2)));

var_dump(iterator_to_array($cursor));

?>
===DONE===
<?php exit(0); ?>
--EXPECT--
array(0) {
}
===DONE===
