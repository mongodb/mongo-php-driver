--TEST--
Causal consistency: unacknowledged write does not update operationTime
--SKIPIF--
<?php require __DIR__ . "/../utils/basic-skipif.inc"; ?>
<?php skip_if_not_libmongoc_crypto(); ?>
<?php skip_if_not_replica_set_or_mongos_with_replica_set(); ?>
<?php skip_if_server_version('<', '3.6'); ?>
<?php skip_if_not_clean(); ?>
--FILE--
<?php
require_once __DIR__ . "/../utils/basic.inc";

$manager = new MongoDB\Driver\Manager(URI);
$session = $manager->startSession();

echo "Initial operation time:\n";
var_dump($session->getOperationTime());

$bulk = new MongoDB\Driver\BulkWrite;
$bulk->insert(['x' => 1]);
$writeConcern = new MongoDB\Driver\WriteConcern(0);

/* Ignore the InvalidArgumentException for trying to combine an unacknowledged
 * write concern with an explicit session. */
try {
    $manager->executeBulkWrite(NS, $bulk, ['session' => $session, 'writeConcern' => $writeConcern]);
} catch (MongoDB\Driver\Exception\InvalidArgumentException $e) {}

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
