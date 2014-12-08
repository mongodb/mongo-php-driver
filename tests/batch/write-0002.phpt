--TEST--
MongoDB\Write\Batch: #002 Get the generated ID
--SKIPIF--
<?php require "tests/utils/basic-skipif.inc"?>
--FILE--
<?php 
require_once "tests/utils/basic.inc";

$mm = new MongoDB\Manager(MONGODB_URI);

$hannes = array("name" => "Hannes", "country" => "USA", "gender" => "male");
$hayley = array("name" => "Hayley", "country" => "USA", "gender" => "female");
$ordered = true;

$insertBatch = new \MongoDB\WriteBatch($ordered);
$hannes_id = $insertBatch->insert($hannes);
$hayley_id = $insertBatch->insert($hayley);

$w = 1;
$wtimeout = 1000;
$writeConcern = new \MongoDB\WriteConcern($w, $wtimeout);
$result = $mm->executeWriteBatch("db.collection", $insertBatch, $writeConcern);

assert($result instanceof \MongoDB\WriteResult);

printf(
    "Inserted %d documents to %s\n",
    $result->getInsertedCount(),
    $result->getServer()->getHost()
);
printf("hannes: %s\nhayley: %s\n", $hannes_id, $hayley_id);
?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
Inserted 2 documents to localhost
hannes: %s
hayley: %s
===DONE===
