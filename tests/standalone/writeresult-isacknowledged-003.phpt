--TEST--
MongoDB\Driver\WriteResult::isAcknowledged() with custom WriteConcern
--SKIPIF--
<?php require __DIR__ . "/../utils/basic-skipif.inc"; ?>
<?php skip_if_not_live(); ?>
<?php skip_if_not_clean(); ?>
--FILE--
<?php
require_once __DIR__ . "/../utils/basic.inc";

$manager = new MongoDB\Driver\Manager(URI);

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
  NULL
  ["nMatched"]=>
  NULL
  ["nModified"]=>
  NULL
  ["nRemoved"]=>
  NULL
  ["nUpserted"]=>
  NULL
  ["upsertedIds"]=>
  array(0) {
  }
  ["writeErrors"]=>
  array(0) {
  }
  ["writeConcernError"]=>
  NULL
  ["writeConcern"]=>
  object(MongoDB\Driver\WriteConcern)#%d (%d) {
    ["w"]=>
    int(0)
  }
}
===DONE===
