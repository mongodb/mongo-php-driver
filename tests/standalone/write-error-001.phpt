--TEST--
MongoDB\Driver\Manager::executeInsert()
--SKIPIF--
<?php require "tests/utils/basic-skipif.inc"; CLEANUP(STANDALONE) ?>
--FILE--
<?php
require_once "tests/utils/basic.inc";

$manager = new MongoDB\Driver\Manager(STANDALONE);

$hannes = array("name" => "Hannes", "country" => "USA", "gender" => "male");
$ordered = true;

$bulk = new \MongoDB\Driver\BulkWrite($ordered);
$hannes_id = $bulk->insert($hannes);

$w = 2;
$wtimeout = 1000;
$writeConcern = new \MongoDB\Driver\WriteConcern($w, $wtimeout);
throws(function() use($bulk, $writeConcern, $manager) {
    $result = $manager->executeBulkWrite("db.collection", $bulk, $writeConcern);
}, "MongoDB\Driver\ConnectionException");

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
OK: Got MongoDB\Driver\ConnectionException
===DONE===
