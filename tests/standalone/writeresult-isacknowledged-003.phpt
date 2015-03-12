--TEST--
MongoDB\Driver\WriteResult::isAcknowledged() with custom WriteConcern
--SKIPIF--
<?php require "tests/utils/basic-skipif.inc" ?>
--FILE--
<?php
require_once "tests/utils/basic.inc";

$manager = new MongoDB\Driver\Manager(MONGODB_URI);

$result = $manager->executeInsert(NS, array('x' => 2), new MongoDB\Driver\WriteConcern(0));

printf("WriteResult::isAcknowledged(): %s\n", $result->isAcknowledged() ? 'true' : 'false');
var_dump($result);

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
WriteResult::isAcknowledged(): false
object(MongoDB\Driver\WriteResult)#%d (%d) {
  ["nInserted"]=>
  int(0)
  ["nMatched"]=>
  int(0)
  ["nModified"]=>
  int(0)
  ["nRemoved"]=>
  int(0)
  ["nUpserted"]=>
  int(0)
  ["upsertedIds"]=>
  array(0) {
  }
  ["writeErrors"]=>
  array(0) {
  }
  ["writeConcernError"]=>
  array(0) {
  }
  ["writeConcern"]=>
  array(5) {
    ["w"]=>
    int(0)
    ["wmajority"]=>
    bool(false)
    ["wtimeout"]=>
    int(0)
    ["fsync"]=>
    bool(false)
    ["journal"]=>
    bool(false)
  }
}
===DONE===
