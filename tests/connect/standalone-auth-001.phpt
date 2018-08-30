--TEST--
Connect to MongoDB with using default auth mechanism
--SKIPIF--
<?php require __DIR__ . "/../utils/basic-skipif.inc"; ?>
<?php skip_if_not_auth(); ?>
<?php skip_if_not_auth_mechanism(null); ?>
<?php skip_if_not_clean(); ?>
--FILE--
<?php
require_once __DIR__ . "/../utils/basic.inc";

$manager = new MongoDB\Driver\Manager(URI);

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
