--TEST--
MongoDB\Driver\WriteResult::isAcknowledged() with custom WriteConcern
--SKIPIF--
<?php require __DIR__ . "/../utils/basic-skipif.inc"; ?>
<?php skip_if_not_live(); ?>
<?php skip_if_not_clean(); ?>
--FILE--
<?php
require_once __DIR__ . "/../utils/basic.inc";

$manager = create_test_manager();

$bulk = new \MongoDB\Driver\BulkWrite;
$bulk->insert(array('x' => 2));
$result = $manager->executeBulkWrite(NS, $bulk, ['writeConcern' => new MongoDB\Driver\WriteConcern(0)]);

printf("WriteResult::isAcknowledged(): %s\n", $result->isAcknowledged() ? 'true' : 'false');
var_dump($result);

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
WriteResult::isAcknowledged(): false
object(MongoDB\Driver\WriteResult)#%d (%d) {
  ["insertedCount"]=>
  NULL
  ["matchedCount"]=>
  NULL
  ["modifiedCount"]=>
  NULL
  ["deletedCount"]=>
  NULL
  ["upsertedCount"]=>
  NULL
  ["server"]=>
  object(MongoDB\Driver\Server)#%d (%d) {%A
  }
  ["upsertedIds"]=>
  array(0) {
  }
  ["writeErrors"]=>
  array(0) {
  }
  ["writeConcernError"]=>
  NULL
  ["writeConcern"]=>
  object(MongoDB\Driver\WriteConcern)#%d (3) {
    ["w"]=>
    int(0)
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
