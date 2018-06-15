--TEST--
MongoDB\Driver\BulkWrite: #002 Get the generated ID
--SKIPIF--
<?php require __DIR__ . "/../utils/basic-skipif.inc"; ?>
<?php skip_if_not_live(); ?>
<?php skip_if_not_clean(); ?>
--FILE--
<?php
require_once __DIR__ . "/../utils/basic.inc";

$manager = new MongoDB\Driver\Manager(URI);

$hannes = array("name" => "Hannes", "country" => "USA", "gender" => "male");
$hayley = array("name" => "Bayley", "country" => "USA", "gender" => "female");

$insertBulk = new \MongoDB\Driver\BulkWrite(['ordered' => true]);
$hannes_id = $insertBulk->insert($hannes);
$hayley_id = $insertBulk->insert($hayley);

$w = 1;
$wtimeout = 1000;
$writeConcern = new \MongoDB\Driver\WriteConcern($w, $wtimeout);
var_dump($insertBulk);
$result = $manager->executeBulkWrite(NS, $insertBulk, $writeConcern);
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
  ["bypassDocumentValidation"]=>
  NULL
  ["executed"]=>
  bool(false)
  ["server_id"]=>
  int(0)
  ["write_concern"]=>
  NULL
}
object(MongoDB\Driver\BulkWrite)#%d (%d) {
  ["database"]=>
  string(6) "phongo"
  ["collection"]=>
  string(15) "bulk_write_0002"
  ["ordered"]=>
  bool(true)
  ["bypassDocumentValidation"]=>
  NULL
  ["executed"]=>
  bool(true)
  ["server_id"]=>
  int(%r[1-9]\d*%r)
  ["write_concern"]=>
  array(%d) {
    ["w"]=>
    int(1)
    ["wtimeout"]=>
    int(1000)
  }
}
Inserted 2 documents to %s
hannes: %s
hayley: %s
===DONE===
