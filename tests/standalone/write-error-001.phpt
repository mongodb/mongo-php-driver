--TEST--
MongoDB\Driver\Manager::executeInsert()
--SKIPIF--
<?php require __DIR__ . "/../utils/basic-skipif.inc"; ?>
<?php NEEDS('STANDALONE'); CLEANUP(STANDALONE); ?>
--FILE--
<?php
require_once __DIR__ . "/../utils/basic.inc";

$manager = new MongoDB\Driver\Manager(STANDALONE);

$hannes = array("name" => "Hannes", "country" => "USA", "gender" => "male");

$bulk = new \MongoDB\Driver\BulkWrite(['ordered' => true]);
$hannes_id = $bulk->insert($hannes);

$w = 2;
$wtimeout = 1000;
$writeConcern = new \MongoDB\Driver\WriteConcern($w, $wtimeout);
throws(function() use($bulk, $writeConcern, $manager) {
    $result = $manager->executeBulkWrite(NS, $bulk, $writeConcern);
}, "MongoDB\Driver\Exception\ConnectionException");

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
OK: Got MongoDB\Driver\Exception\ConnectionException
===DONE===
