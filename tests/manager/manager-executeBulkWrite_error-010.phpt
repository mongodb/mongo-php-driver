--TEST--
MongoDB\Driver\Manager::executeBulkWrite() with unknown options
--XFAIL--
Depends on PHPC-1066
--SKIPIF--
<?php require __DIR__ . "/../utils/basic-skipif.inc"; ?>
<?php NEEDS('REPLICASET'); ?>
--FILE--
<?php
require_once __DIR__ . "/../utils/basic.inc";

$manager = new MongoDB\Driver\Manager(REPLICASET);

echo throws(function() use ($manager) {
    $bulk = new MongoDB\Driver\BulkWrite();
    $bulk->insert(['x' => 1]);
    $manager->executeBulkWrite(NS, $bulk, ['readConcern' => 'foo']);
}, 'MongoDB\Driver\Exception\InvalidArgumentException'), "\n";

echo throws(function() use ($manager) {
    $bulk = new MongoDB\Driver\BulkWrite();
    $bulk->insert(['x' => 1]);
    $manager->executeBulkWrite(NS, $bulk, ['readPreference' => 'foo']);
}, 'MongoDB\Driver\Exception\InvalidArgumentException'), "\n";

echo throws(function() use ($manager) {
    $bulk = new MongoDB\Driver\BulkWrite();
    $bulk->insert(['x' => 1]);
    $manager->executeBulkWrite(NS, $bulk, ['unknown' => 'foo']);
}, 'MongoDB\Driver\Exception\InvalidArgumentException'), "\n";

?>
===DONE===
<?php exit(0); ?>
--EXPECT--
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Unknown option 'readConcern'
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Unknown option 'readPreference'
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Unknown option 'unknown'
===DONE===
