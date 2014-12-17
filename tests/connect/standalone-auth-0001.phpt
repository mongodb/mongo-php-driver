--TEST--
MongoDB\Write\Batch: #001 Variety Batch
--SKIPIF--
<?php exit("skip FIXME: https://github.com/10gen/mongo-orchestration/issues/154"); ?>
<?php require "tests/utils/basic-skipif.inc"?>
--FILE--
<?php 
require_once "tests/utils/basic.inc";

$username = "root";
$password = "toor";
$database = "admin";

$parsed = parse_url(MONGODB_STANDALONE_AUTH_URI);
$dsn = sprintf("mongodb://%s:%s@%s:%d/%s", $username, $password, $parsed["host"], $parsed["port"], $database);
var_dump($dsn);
$mc = new MongoDB\Manager($dsn);

$batch = new MongoDB\WriteBatch;

$batch->insert(array("my" => "value"));
$batch->insert(array("my" => "value", "foo" => "bar"));
$batch->insert(array("my" => "value", "foo" => "bar"));

$batch->delete(array("my" => "value", "foo" => "bar"), array("limit" => 1));

$batch->update(array("foo" => "bar"), array('$set' => array("foo" => "baz")), array("limit" => 1, "upsert" => 0));

$retval = $mc->executeWriteBatch(NS, $batch);

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
