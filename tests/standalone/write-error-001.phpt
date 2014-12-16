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
try {
    $result = $manager->executeWriteBatch("db.collection", $insertBatch, $writeConcern);
} catch(MongoDB\WriteException $e) {
    printWriteResult($e->getWriteResult());
}

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
server: %s:%d
insertedCount: 0
matchedCount: 0
modifiedCount: 0
upsertedCount: 0
deletedCount: 0
===DONE===
