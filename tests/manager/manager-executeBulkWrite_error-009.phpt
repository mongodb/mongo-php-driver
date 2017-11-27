--TEST--
MongoDB\Driver\Manager::executeBulkWrite() with wrong options and values
--SKIPIF--
<?php require __DIR__ . "/../utils/basic-skipif.inc"; ?>
<?php NEEDS('REPLICASET'); ?>
--FILE--
<?php
require_once __DIR__ . "/../utils/basic.inc";

$manager = new MongoDB\Driver\Manager(REPLICASET);
    
echo throws(function() use ($manager) {
    $bulk = new MongoDB\Driver\BulkWrite();
    $bulk->insert(['wc' => 1]);
    $manager->executeBulkWrite(NS, $bulk, ['writeConcern' => 'foo']);
}, 'MongoDB\Driver\Exception\InvalidArgumentException'), "\n";

echo throws(function() use ($manager) {
    $bulk = new MongoDB\Driver\BulkWrite();
    $bulk->insert(['wc' => 1]);
    $manager->executeBulkWrite(NS, $bulk, ['writeConcern' => 42]);
}, 'MongoDB\Driver\Exception\InvalidArgumentException'), "\n";

echo throws(function() use ($manager) {
    $bulk = new MongoDB\Driver\BulkWrite();
    $bulk->insert(['wc' => 1]);
    $manager->executeBulkWrite(NS, $bulk, ['unknown' => 'foo']);
}, 'MongoDB\Driver\Exception\InvalidArgumentException'), "\n";

echo throws(function() use ($manager) {
    $bulk = new MongoDB\Driver\BulkWrite();
    $bulk->insert(['wc' => 1]);
    $manager->executeBulkWrite(NS, $bulk, ['readPreference' => 'foo']);
}, 'MongoDB\Driver\Exception\InvalidArgumentException'), "\n";

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Expected 'writeConcern' option to be 'MongoDB\Driver\WriteConcern', string given
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Expected 'writeConcern' option to be 'MongoDB\Driver\WriteConcern', integer given
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Unknown option 'unknown'
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Unknown option 'readPreference'
===DONE===
