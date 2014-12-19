--TEST--
MongoDB\Manager::executeInsert()
--SKIPIF--
<?php require "tests/utils/basic-skipif.inc" ?>
--FILE--
<?php
require_once "tests/utils/basic.inc";

$manager = new MongoDB\Manager(MONGODB_URI);

$hannes = array("name" => "Hannes", "country" => "USA", "gender" => "male");
$ordered = true;

$insertBatch = new \MongoDB\WriteBatch($ordered);
$hannes_id = $insertBatch->insert($hannes);

$w = 2;
$wtimeout = 1000;
$writeConcern = new \MongoDB\WriteConcern($w, $wtimeout);
throws(function() use($insertBatch, $writeConcern, $manager) {
    $result = $manager->executeWriteBatch("db.collection", $insertBatch, $writeConcern);
}, "MongoDB\ConnectionException");

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
OK: Got MongoDB\ConnectionException
===DONE===
