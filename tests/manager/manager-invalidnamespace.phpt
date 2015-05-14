--TEST--
MongoDB\Driver\Manager: Invalid namespace
--SKIPIF--
<?php require __DIR__ . "/../utils/basic-skipif.inc"?>
--FILE--
<?php
require_once __DIR__ . "/../utils/basic.inc";

$manager = new MongoDB\Driver\Manager(STANDALONE);
$bulk = new MongoDB\Driver\BulkWrite;
$bulk->insert(array("my" => "value"));

throws(function() use($manager, $bulk) {
    $retval = $manager->executeBulkWrite("database", $bulk);
}, "MongoDB\Driver\Exception\InvalidArgumentException");

throws(function() use($manager) {
    $retval = $manager->executeInsert("database", array("document "=> 1));
}, "MongoDB\Driver\Exception\InvalidArgumentException");
throws(function() use($manager) {
    $retval = $manager->executeQuery("database", new MongoDB\Driver\Query(array("document "=> 1)));
}, "MongoDB\Driver\Exception\InvalidArgumentException");
?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
===DONE===
