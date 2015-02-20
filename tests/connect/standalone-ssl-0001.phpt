--TEST--
Connect to MongoDB with using SSL
--SKIPIF--
<?php require "tests/utils/basic-skipif.inc"?>
--FILE--
<?php
require_once "tests/utils/basic.inc";

$dsn = sprintf("%s/?ssl=true", MONGODB_STANDALONE_SSL_URI);

$manager = new MongoDB\Driver\Manager($dsn);

$bulk = new MongoDB\Driver\BulkWrite;

$bulk->insert(array("my" => "value"));
$bulk->insert(array("my" => "value", "foo" => "bar"));
$bulk->insert(array("my" => "value", "foo" => "bar"));

$bulk->delete(array("my" => "value", "foo" => "bar"), array("limit" => 1));

$bulk->update(array("foo" => "bar"), array('$set' => array("foo" => "baz")), array("limit" => 1, "upsert" => 0));

$retval = $manager->executeBulkWrite(NS, $bulk);

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
