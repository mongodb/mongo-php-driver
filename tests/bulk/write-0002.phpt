--TEST--
MongoDB\Driver\BulkWrite: #002 Get the generated ID
--SKIPIF--
<?php require __DIR__ . "/../utils/basic-skipif.inc"?>
--FILE--
<?php
require_once __DIR__ . "/../utils/basic.inc";

$manager = new MongoDB\Driver\Manager(STANDALONE);

$hannes = array("name" => "Hannes", "country" => "USA", "gender" => "male");
$hayley = array("name" => "Hayley", "country" => "USA", "gender" => "female");

$insertBulk = new \MongoDB\Driver\BulkWrite(['ordered' => true]);
$hannes_id = $insertBulk->insert($hannes);
$hayley_id = $insertBulk->insert($hayley);

$w = 1;
$wtimeout = 1000;
$writeConcern = new \MongoDB\Driver\WriteConcern($w, $wtimeout);
var_dump($insertBulk);
$result = $manager->executeBulkWrite("db.collection", $insertBulk, $writeConcern);
var_dump($insertBulk);

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
object(MongoDB\Driver\BulkWrite)#%d (%d) {
  ["database"]=>
  NULL
  ["collection"]=>
  NULL
  ["ordered"]=>
  bool(true)
  ["executed"]=>
  bool(false)
  ["server_id"]=>
  int(0)
  ["write_concern"]=>
  NULL
}
object(MongoDB\Driver\BulkWrite)#%d (%d) {
  ["database"]=>
  string(2) "db"
  ["collection"]=>
  string(10) "collection"
  ["ordered"]=>
  bool(true)
  ["executed"]=>
  bool(true)
  ["server_id"]=>
  int(1)
  ["write_concern"]=>
  array(%d) {
    ["w"]=>
    int(1)
    ["wmajority"]=>
    bool(false)
    ["wtimeout"]=>
    int(1000)
    ["journal"]=>
    NULL
  }
}
Inserted 2 documents to %s
hannes: %s
hayley: %s
===DONE===
