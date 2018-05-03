--TEST--
WriteConcernError: Populate WriteConcernError on WriteConcern errors
--SKIPIF--
<?php require __DIR__ . "/../utils/basic-skipif.inc"; ?>
<?php skip_if_not_replica_set(); ?>
--FILE--
<?php
require_once __DIR__ . "/../utils/basic.inc";

$manager = new MongoDB\Driver\Manager(URI);

$bulk = new MongoDB\Driver\BulkWrite;

$bulk->insert(array("my" => "value"));

$w = new MongoDB\Driver\WriteConcern(30, 100);
try {
    $retval = $manager->executeBulkWrite(NS, $bulk, $w);
} catch(MongoDB\Driver\Exception\BulkWriteException $e) {
    $server = $e->getWriteResult()->getServer();
    $server->getPort();
    printWriteResult($e->getWriteResult(), false);
}
?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
server: %s:%d
insertedCount: 1
matchedCount: 0
modifiedCount: 0
upsertedCount: 0
deletedCount: 0
writeConcernError: %s (%d)
===DONE===
