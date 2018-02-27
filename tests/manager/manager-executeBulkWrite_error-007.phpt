--TEST--
MongoDB\Driver\Manager::executeBulkWrite() should not issue warning before exception
--FILE--
<?php
require_once __DIR__ . "/../utils/basic.inc";

// Invalid host cannot be resolved
$manager = new MongoDB\Driver\Manager('mongodb://invalid.host:27017', ['serverSelectionTimeoutMS' => 1]);

echo throws(function() use ($manager) {
    $bulk = new MongoDB\Driver\BulkWrite;
    $bulk->insert(['x' => 1]);
    $manager->executeBulkWrite(NS, $bulk);
}, 'MongoDB\Driver\Exception\ConnectionTimeoutException'), "\n";

// Valid host refuses connection
$manager = new MongoDB\Driver\Manager('mongodb://localhost:54321', ['serverSelectionTimeoutMS' => 1]);

echo throws(function() use ($manager) {
    $bulk = new MongoDB\Driver\BulkWrite;
    $bulk->insert(['x' => 1]);
    $manager->executeBulkWrite(NS, $bulk);
}, 'MongoDB\Driver\Exception\ConnectionTimeoutException'), "\n";

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
OK: Got MongoDB\Driver\Exception\ConnectionTimeoutException
No suitable servers found (`serverSelectionTryOnce` set): %s
OK: Got MongoDB\Driver\Exception\ConnectionTimeoutException
No suitable servers found (`serverSelectionTryOnce` set): %s
===DONE===
