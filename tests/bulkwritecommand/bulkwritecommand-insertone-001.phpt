--TEST--
MongoDB\Driver\BulkWriteCommand::insertOne() returns document ID
--SKIPIF--
<?php require __DIR__ . "/../utils/basic-skipif.inc"; ?>
--FILE--
<?php

require_once __DIR__ . "/../utils/basic.inc";

$bulk = new MongoDB\Driver\BulkWriteCommand();

var_dump($bulk->insertOne(NS, ['_id' => 1]));
var_dump($bulk->insertOne(NS, []));


?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
int(1)
object(MongoDB\BSON\ObjectId)#%d (%d) {
  ["oid"]=>
  string(24) "%x"
}
===DONE===
