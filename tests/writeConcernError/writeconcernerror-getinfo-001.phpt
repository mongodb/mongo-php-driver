--TEST--
MongoDB\Driver\WriteConcernError::getInfo()
--SKIPIF--
<?php require __DIR__ . "/../utils/basic-skipif.inc"; ?>
<?php NEEDS('REPLICASET_30'); CLEANUP(REPLICASET_30); ?>
--FILE--
<?php
require_once __DIR__ . "/../utils/basic.inc";

$manager = new MongoDB\Driver\Manager(REPLICASET_30);

$bulk = new MongoDB\Driver\BulkWrite;
$bulk->insert(['x' => 1]);

try {
    /* We assume that the replica set does not have 12 nodes */
    $manager->executeBulkWrite(NS, $bulk, new MongoDB\Driver\WriteConcern(12));
} catch(MongoDB\Driver\Exception\BulkWriteException $e) {
    var_dump($e->getWriteResult()->getWriteConcernError()->getInfo());
}

?>
===DONE===
<?php exit(0); ?>
--EXPECT--
NULL
===DONE===
