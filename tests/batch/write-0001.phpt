--TEST--
MongoDB\Write\Batch: #001 Variety Batch
--SKIPIF--
<?php require "tests/utils/basic-skipif.inc"?>
--FILE--
<?php 
require_once "tests/utils/basic.inc";

$mc = new MongoDB\Manager(MONGODB_URI);

$batch = new MongoDB\Write\Batch;

$batch->insert(array("my" => "value"));
$batch->insert(array("my" => "value", "foo" => "bar"));
$batch->insert(array("my" => "value", "foo" => "bar"));

$batch->delete(array("my" => "value", "foo" => "bar"), 1);

$batch->update(array("foo" => "bar"), array('$set' => array("foo" => "baz")), 1, 1);

$retval = $mc->executeWrite(NS, $batch);

printf("Inserted: %d\n", getInsertCount($retval));
printf("Deleted: %d\n", getRemovedCount($retval));
printf("Updated: %d\n", getModifiedCount($retval));
printf("Upserted: %d\n", getUpsertedCount($retval));
foreach(getWriteErrors($retval) as $error) {
    printf("WriteErrors: %", $error);
}
?>
===DONE===
<?php exit(0); ?>
--EXPECT--
Inserted: 3
Deleted: 1
Updated: 1
Upserted: 0
===DONE===
