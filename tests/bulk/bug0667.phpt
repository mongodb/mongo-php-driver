--TEST--
PHPC-667: BulkWrite::insert() does not generate ObjectId if another field has "_id" prefix
--FILE--
<?php

$bulk = new MongoDB\Driver\BulkWrite;
var_dump($bulk->insert(['_ids' => 1]));
var_dump($bulk->insert((object) ['_ids' => 1]));

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
object(MongoDB\BSON\ObjectId)#%d (%d) {
  ["oid"]=>
  string(24) "%x"
}
object(MongoDB\BSON\ObjectId)#%d (%d) {
  ["oid"]=>
  string(24) "%x"
}
===DONE===
