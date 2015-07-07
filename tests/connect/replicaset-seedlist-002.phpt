--TEST--
MongoDB\Driver\Manager: Connecting to Replica Set with only arbiter in seedlist
--SKIPIF--
<?php require __DIR__ . "/../utils/basic-skipif.inc"; NEEDS("REPLICASET"); ?>
<?php CLEANUP(REPLICASET); PREDICTABLE(); ?>
--FILE--
<?php
require_once __DIR__ . "/../utils/basic.inc";


$dsn = "mongodb://192.168.112.10:3002/?replicaSet=REPLICASET";
$manager = new MongoDB\Driver\Manager($dsn);

// load fixtures for test
$bulk = new \MongoDB\Driver\BulkWrite();
$bulk->insert(array("_id" => 1, "x" => 2, "y" => 3));
$bulk->insert(array("_id" => 2, "x" => 3, "y" => 4));
$bulk->insert(array("_id" => 3, "x" => 4, "y" => 5));
$manager->executeBulkWrite(NS, $bulk);
?>
===DONE===
<?php exit(0); ?>
--EXPECT--
===DONE===
