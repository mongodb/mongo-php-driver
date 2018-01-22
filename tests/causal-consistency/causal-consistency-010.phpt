--TEST--
Causal consistency: unacknowledged write does not update operationTime
--SKIPIF--
<?php require __DIR__ . "/../utils/basic-skipif.inc"; ?>
<?php NEEDS_CRYPTO(); ?>
<?php NEEDS('REPLICASET'); CLEANUP(REPLICASET); ?>
--FILE--
<?php
require_once __DIR__ . "/../utils/basic.inc";

$manager = new MongoDB\Driver\Manager(REPLICASET);
$session = $manager->startSession();

echo "Initial operation time:\n";
var_dump($session->getOperationTime());

$bulk = new MongoDB\Driver\BulkWrite;
$bulk->insert(['x' => 1]);
$writeConcern = new MongoDB\Driver\WriteConcern(0);
$manager->executeBulkWrite(NS, $bulk, ['session' => $session, 'writeConcern' => $writeConcern]);

echo "\nOperation time after unacknowledged write:\n";
var_dump($session->getOperationTime());

?>
===DONE===
<?php exit(0); ?>
--EXPECT--
Initial operation time:
NULL

Operation time after unacknowledged write:
NULL
===DONE===
