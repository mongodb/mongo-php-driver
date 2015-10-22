--TEST--
MongoDB\Driver\WriteResult::isAcknowledged() with custom WriteConcern
--SKIPIF--
<?php require __DIR__ . "/../utils/basic-skipif.inc"; CLEANUP(STANDALONE) ?>
--FILE--
<?php
require_once __DIR__ . "/../utils/basic.inc";

$manager = new MongoDB\Driver\Manager(STANDALONE);

$bulk = new \MongoDB\Driver\BulkWrite;
$bulk->insert(array('x' => 2));
$result = $manager->executeBulkWrite(NS, $bulk, new MongoDB\Driver\WriteConcern(0));

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
  NULL
  ["writeConcern"]=>
  array(%d) {
    ["w"]=>
    int(0)
    ["wmajority"]=>
    bool(false)
    ["wtimeout"]=>
    int(0)
    ["journal"]=>
    NULL
  }
}
===DONE===
