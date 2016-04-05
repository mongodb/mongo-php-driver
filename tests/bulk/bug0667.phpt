--TEST--
PHPC-667: BulkWrite::insert() does not generate ObjectID if another field has "_id" prefix
--SKIPIF--
<?php require __DIR__ . "/../utils/basic-skipif.inc"; ?>
--FILE--
<?php
require_once __DIR__ . "/../utils/basic.inc";

$bulk = new MongoDB\Driver\BulkWrite;
var_dump($bulk->insert(['_ids' => 1]));
var_dump($bulk->insert((object) ['_ids' => 1]));

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
object(MongoDB\BSON\ObjectID)#%d (%d) {
  ["oid"]=>
  string(24) "%x"
}
object(MongoDB\BSON\ObjectID)#%d (%d) {
  ["oid"]=>
  string(24) "%x"
}
===DONE===
