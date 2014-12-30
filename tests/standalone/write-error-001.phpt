--TEST--
MongoDB\Driver\Manager::executeInsert()
--SKIPIF--
<?php require "tests/utils/basic-skipif.inc" ?>
--FILE--
<?php
require_once "tests/utils/basic.inc";

$manager = new MongoDB\Driver\Manager(MONGODB_URI);

$hannes = array("name" => "Hannes", "country" => "USA", "gender" => "male");
$ordered = true;

$insertBatch = new \MongoDB\Driver\WriteBatch($ordered);
$hannes_id = $insertBatch->insert($hannes);

$w = 2;
$wtimeout = 1000;
$writeConcern = new \MongoDB\Driver\WriteConcern($w, $wtimeout);
throws(function() use($insertBatch, $writeConcern, $manager) {
    $result = $manager->executeWriteBatch("db.collection", $insertBatch, $writeConcern);
}, "MongoDB\Driver\ConnectionException");

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
OK: Got MongoDB\Driver\ConnectionException
===DONE===
