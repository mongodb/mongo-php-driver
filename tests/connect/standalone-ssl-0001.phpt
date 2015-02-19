--TEST--
Connect to MongoDB with using SSL
--SKIPIF--
<?php require "tests/utils/basic-skipif.inc"?>
--FILE--
<?php
require_once "tests/utils/basic.inc";

$dsn = sprintf("%s/?ssl=true", MONGODB_STANDALONE_SSL_URI);

$manager = new MongoDB\Driver\Manager($dsn);

$batch = new MongoDB\Driver\WriteBatch;

$batch->insert(array("my" => "value"));
$batch->insert(array("my" => "value", "foo" => "bar"));
$batch->insert(array("my" => "value", "foo" => "bar"));

$batch->delete(array("my" => "value", "foo" => "bar"), array("limit" => 1));

$batch->update(array("foo" => "bar"), array('$set' => array("foo" => "baz")), array("limit" => 1, "upsert" => 0));

$retval = $manager->executeWriteBatch(NS, $batch);

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
--EXPECT--
Inserted: 3
Deleted: 1
Updated: 1
Upserted: 0
===DONE===
