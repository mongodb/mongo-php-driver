--TEST--
Connect to MongoDB with using SSL without verifying anything
--SKIPIF--
<?php require __DIR__ . "/../utils/basic-skipif.inc"; NEEDS("STANDALONE_SSL"); ?>
--FILE--
<?php
require_once __DIR__ . "/" . "../utils/basic.inc";

$SSL_DIR = realpath(__DIR__ . "/" . "./../../scripts/ssl/");
$opts = array(
    "ssl" => array(
        "verify_peer" => false,
        "verify_peer_name" => false,
        "allow_self_signed" => true,
    ),
);
$context = stream_context_create($opts);

$dsn = sprintf("%s/?ssl=true", STANDALONE_SSL);

$manager = new MongoDB\Driver\Manager($dsn, array(), array("context" => $context));

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
