--TEST--
MongoDB\Driver\Write\Batch: #001 Variety Batch
--SKIPIF--
<?php require "tests/utils/basic-skipif.inc"?>
--FILE--
<?php
require_once "tests/utils/basic.inc";

$manager = new MongoDB\Driver\Manager(MONGODB_URI);

$batch = new MongoDB\Driver\WriteBatch;
var_dump($batch);

$batch->insert(array("my" => "value"));
$batch->insert(array("my" => "value", "foo" => "bar"));
$batch->insert(array("my" => "value", "foo" => "bar"));
var_dump($batch);

$batch->delete(array("my" => "value", "foo" => "bar"), array("limit" => 1));
var_dump($batch);

$batch->update(array("foo" => "bar"), array('$set' => array("foo" => "baz")), array("limit" => 1, "upsert" => 0));

var_dump($batch);

$retval = $manager->executeWriteBatch(NS, $batch);

var_dump($batch);

printf("Inserted: %d\n", getInsertCount($retval));
printf("Deleted: %d\n", getDeletedCount($retval));
printf("Updated: %d\n", getModifiedCount($retval));
printf("Upserted: %d\n", getUpsertedCount($retval));
foreach(getWriteErrors($retval) as $error) {
    printf("WriteErrors: %", $error);
}
?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
object(MongoDB\Driver\WriteBatch)#%d (%d) {
  ["database"]=>
  NULL
  ["collection"]=>
  NULL
  ["ordered"]=>
  bool(true)
  ["executed"]=>
  bool(false)
  ["hint"]=>
  int(0)
  ["write_concern"]=>
  NULL
}
object(MongoDB\Driver\WriteBatch)#%d (%d) {
  ["database"]=>
  NULL
  ["collection"]=>
  NULL
  ["ordered"]=>
  bool(true)
  ["executed"]=>
  bool(false)
  ["hint"]=>
  int(0)
  ["write_concern"]=>
  NULL
}
object(MongoDB\Driver\WriteBatch)#%d (%d) {
  ["database"]=>
  NULL
  ["collection"]=>
  NULL
  ["ordered"]=>
  bool(true)
  ["executed"]=>
  bool(false)
  ["hint"]=>
  int(0)
  ["write_concern"]=>
  NULL
}
object(MongoDB\Driver\WriteBatch)#%d (%d) {
  ["database"]=>
  NULL
  ["collection"]=>
  NULL
  ["ordered"]=>
  bool(true)
  ["executed"]=>
  bool(false)
  ["hint"]=>
  int(0)
  ["write_concern"]=>
  NULL
}
object(MongoDB\Driver\WriteBatch)#%d (%d) {
  ["database"]=>
  string(6) "phongo"
  ["collection"]=>
  string(16) "batch_write_0001"
  ["ordered"]=>
  bool(true)
  ["executed"]=>
  bool(true)
  ["hint"]=>
  int(0)
  ["write_concern"]=>
  array(0) {
  }
}
Inserted: 3
Deleted: 1
Updated: 1
Upserted: 0
===DONE===
