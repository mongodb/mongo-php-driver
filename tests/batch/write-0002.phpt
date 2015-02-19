--TEST--
MongoDB\Driver\Write\Batch: #002 Get the generated ID
--SKIPIF--
<?php require "tests/utils/basic-skipif.inc"?>
--FILE--
<?php
require_once "tests/utils/basic.inc";

$manager = new MongoDB\Driver\Manager(MONGODB_URI);

$hannes = array("name" => "Hannes", "country" => "USA", "gender" => "male");
$hayley = array("name" => "Hayley", "country" => "USA", "gender" => "female");
$ordered = true;

$insertBatch = new \MongoDB\Driver\WriteBatch($ordered);
$hannes_id = $insertBatch->insert($hannes);
$hayley_id = $insertBatch->insert($hayley);

$w = 1;
$wtimeout = 1000;
$writeConcern = new \MongoDB\Driver\WriteConcern($w, $wtimeout);
var_dump($insertBatch);
$result = $manager->executeWriteBatch("db.collection", $insertBatch, $writeConcern);
var_dump($insertBatch);

assert($result instanceof \MongoDB\Driver\WriteResult);

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
object(MongoDB\Driver\WriteBatch)#%d (%d) {
  ["database"]=>
  NULL
  ["collection"]=>
  NULL
  ["ordered"]=>
  bool(true)
  ["executed"]=>
  bool(false)
  ["hint"]=>
  int(0)
  ["write_concern"]=>
  NULL
}
object(MongoDB\Driver\WriteBatch)#%d (%d) {
  ["database"]=>
  string(2) "db"
  ["collection"]=>
  string(10) "collection"
  ["ordered"]=>
  bool(true)
  ["executed"]=>
  bool(true)
  ["hint"]=>
  int(0)
  ["write_concern"]=>
  array(5) {
    ["w"]=>
    int(1)
    ["wmajority"]=>
    bool(false)
    ["wtimeout"]=>
    int(1000)
    ["fsync"]=>
    bool(false)
    ["journal"]=>
    bool(false)
  }
}
Inserted 2 documents to %s
hannes: %s
hayley: %s
===DONE===
