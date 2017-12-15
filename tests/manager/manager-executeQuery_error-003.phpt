--TEST--
MongoDB\Driver\Manager::executeQuery() with unknown options
--XFAIL--
Depends on PHPC-1066
--SKIPIF--
<?php require __DIR__ . "/../utils/basic-skipif.inc"; ?>
<?php NEEDS('REPLICASET'); CLEANUP(REPLICASET); ?>
--FILE--
<?php
require_once __DIR__ . "/../utils/basic.inc";

$manager = new MongoDB\Driver\Manager(REPLICASET);

$query = new MongoDB\Driver\Query(['x' => 3], ['projection' => ['y' => 1]]);

echo throws(function() use ($manager, $query) {
    $manager->executeQuery(NS, $query, ['readConcern' => 'foo']);
}, 'MongoDB\Driver\Exception\InvalidArgumentException'), "\n";

echo throws(function() use ($manager, $query) {
    $manager->executeQuery(NS, $query, ['writeConcern' => 'foo']);
}, 'MongoDB\Driver\Exception\InvalidArgumentException'), "\n";

echo throws(function() use ($manager, $query) {
    $manager->executeQuery(NS, $query, ['unknown' => 'foo']);
}, 'MongoDB\Driver\Exception\InvalidArgumentException'), "\n";

?>
===DONE===
<?php exit(0); ?>
--EXPECT--
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Unknown option 'readConcern'
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Unknown option 'writeConcern'
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Unknown option 'unknown'
===DONE===
