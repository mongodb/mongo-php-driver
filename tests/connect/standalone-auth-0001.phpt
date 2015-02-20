--TEST--
Connect to MongoDB with using default auth mechanism
--SKIPIF--
<?php require "tests/utils/basic-skipif.inc"?>
--FILE--
<?php
require_once "tests/utils/basic.inc";

$username = "root";
$password = "toor";
$database = "admin";

$parsed = parse_url(MONGODB_STANDALONE_AUTH_URI);
$dsn = sprintf("mongodb://%s:%s@%s:%d/%s", $username, $password, $parsed["host"], $parsed["port"], $database);
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
