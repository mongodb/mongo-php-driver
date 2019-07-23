--TEST--
MongoDB\Driver\Server::executeBulkWrite() with write concern (replica set secondary)
--SKIPIF--
<?php require __DIR__ . "/../utils/basic-skipif.inc"; ?>
<?php skip_if_not_replica_set(); ?>
<?php skip_if_not_enough_data_nodes(2); ?>
<?php skip_if_not_clean(); ?>
--FILE--
<?php
require_once __DIR__ . "/../utils/basic.inc";

/* Disable retryWrites since the test expects to receive a "not master" error,
 * which retryable writes would otherwise use to retry against the primary. */
$manager = new MongoDB\Driver\Manager(URI, ['retryWrites' => false]);
$server = $manager->selectServer(new MongoDB\Driver\ReadPreference(MongoDB\Driver\ReadPreference::RP_SECONDARY));

$writeConcerns = [1, 2, MongoDB\Driver\WriteConcern::MAJORITY];

foreach ($writeConcerns as $wc) {
    $bulk = new MongoDB\Driver\BulkWrite();
    $bulk->insert(['wc' => $wc]);

    $options = [
        'writeConcern' => new MongoDB\Driver\WriteConcern($wc),
    ];

    echo throws(function() use ($server, $bulk, $options) {
        $server->executeBulkWrite(NS, $bulk, $options);
    }, "MongoDB\Driver\Exception\RuntimeException"), "\n";
}

?>
===DONE===
<?php exit(0); ?>
--EXPECT--
OK: Got MongoDB\Driver\Exception\RuntimeException
not master
OK: Got MongoDB\Driver\Exception\RuntimeException
not master
OK: Got MongoDB\Driver\Exception\RuntimeException
not master
===DONE===
