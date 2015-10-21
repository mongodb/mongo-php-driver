--TEST--
MongoDB\Driver\WriteResult::isAcknowledged() with inherited WriteConcern
--SKIPIF--
<?php require __DIR__ . "/../utils/basic-skipif.inc"; CLEANUP(STANDALONE) ?>
--FILE--
<?php
require_once __DIR__ . "/../utils/basic.inc";

/* We use w:0 here because libmongoc detects w:1 as the server's default and
 */
$manager = new MongoDB\Driver\Manager(STANDALONE . "/?w=0");

$bulk = new \MongoDB\Driver\BulkWrite;
$bulk->insert(array('x' => 1));
$result = $manager->executeBulkWrite(NS, $bulk);

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
