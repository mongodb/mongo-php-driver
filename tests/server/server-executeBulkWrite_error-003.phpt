--TEST--
MongoDB\Driver\Server::executeBulkWrite() with unknown options
--XFAIL--
Depends on PHPC-1066
--SKIPIF--
<?php require __DIR__ . "/../utils/basic-skipif.inc"; ?>
<?php NEEDS('REPLICASET'); ?>
--FILE--
<?php
require_once __DIR__ . "/../utils/basic.inc";

$manager = new MongoDB\Driver\Manager(REPLICASET);
$server = $manager->selectServer(new MongoDB\Driver\ReadPreference(MongoDB\Driver\ReadPreference::RP_PRIMARY));

echo throws(function() use ($server) {
    $bulk = new MongoDB\Driver\BulkWrite();
    $bulk->insert(['x' => 1]);
    $server->executeBulkWrite(NS, $bulk, ['readConcern' => 'foo']);
}, 'MongoDB\Driver\Exception\InvalidArgumentException'), "\n";

echo throws(function() use ($server) {
    $bulk = new MongoDB\Driver\BulkWrite();
    $bulk->insert(['x' => 1]);
    $server->executeBulkWrite(NS, $bulk, ['readPreference' => 'foo']);
}, 'MongoDB\Driver\Exception\InvalidArgumentException'), "\n";

echo throws(function() use ($server) {
    $bulk = new MongoDB\Driver\BulkWrite();
    $bulk->insert(['x' => 1]);
    $server->executeBulkWrite(NS, $bulk, ['unknown' => 'foo']);
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
