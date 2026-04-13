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
object(MongoDB\Driver\WriteResult)#%d (%d) {
  ["insertedCount"]=>
  int(1)
  ["matchedCount"]=>
  int(0)
  ["modifiedCount"]=>
  int(0)
  ["deletedCount"]=>
  int(0)
  ["upsertedCount"]=>
  int(0)
  ["server"]=>
  object(MongoDB\Driver\Server)#%d (%d) {%A
  }
  ["upsertedIds"]=>
  array(0) {
  }
  ["writeErrors"]=>
  array(1) {
    [0]=>
    object(MongoDB\Driver\WriteError)#%d (%d) {
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
  object(MongoDB\Driver\WriteConcern)#%d (3) {
    ["w"]=>
    NULL
    ["j"]=>
    NULL
    ["wtimeout"]=>
    int(0)
  }
  ["errorReplies"]=>
  array(0) {
  }
}
===DONE===
