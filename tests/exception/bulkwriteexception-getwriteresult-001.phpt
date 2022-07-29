--TEST--
MongoDB\Driver\Exception\BulkWriteException::getWriteResult()
--SKIPIF--
<?php require __DIR__ . "/../utils/basic-skipif.inc"; ?>
<?php skip_if_not_live(); ?>
<?php skip_if_not_clean(); ?>
--FILE--
<?php
require_once __DIR__ . "/../utils/basic.inc";

$manager = create_test_manager();

try {
    $bulk = new MongoDB\Driver\BulkWrite;
    $bulk->insert(['_id' => 1]);
    $bulk->insert(['_id' => 1]);
    $result = $manager->executeBulkWrite(NS, $bulk);
} catch (MongoDB\Driver\Exception\BulkWriteException $e) {
    var_dump($e->getWriteResult());
}

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
object(MongoDB\Driver\WriteResult)#%d (9) {
  ["nInserted"]=>
  int(1)
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
  array(1) {
    [0]=>
    object(MongoDB\Driver\WriteError)#%d (4) {
      ["message"]=>
      string(%d) %s
      ["code"]=>
      int(11000)
      ["index"]=>
      int(1)
      ["info"]=>
      NULL
    }
  }
  ["writeConcernError"]=>
  NULL
  ["writeConcern"]=>
  object(MongoDB\Driver\WriteConcern)#%d (0) {
  }
}
===DONE===
