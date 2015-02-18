--TEST--
MongoDB\Driver\WriteConcern construction
--SKIPIF--
<?php require "tests/utils/basic-skipif.inc"?>
--FILE--
<?php 
require_once "tests/utils/basic.inc";

$manager = new MongoDB\Driver\Manager(MONGODB_REPLICASET_URI);


$wc = new MongoDB\Driver\WriteConcern("MultipleDC", 500);
 
$doc = array("example" => "document");
try {
    $result = $manager->executeInsert("databaseName.collectionName", $doc, $wc);
} catch(MongoDB\Driver\Exception $e) {
    var_dump(get_class($e), $e->getMessage());
}
var_dump($result->getWriteConcernError());
?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
object(MongoDB\Driver\WriteConcernError)#%d (%d) {
  ["message"]=>
  string(%d) "%s"
  ["code"]=>
  int(79)
  ["info"]=>
  NULL
}
===DONE===
