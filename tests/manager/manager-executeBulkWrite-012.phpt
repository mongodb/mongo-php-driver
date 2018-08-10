--TEST--
MongoDB\Driver\Manager::executeBulkWrite() with legacy write concern (replica set primary)
--SKIPIF--
<?php require __DIR__ . "/../utils/basic-skipif.inc"; ?>
<?php skip_if_not_replica_set(); ?>
<?php skip_if_not_enough_data_nodes(2); ?>
<?php skip_if_not_clean(); ?>
--FILE--
<?php
require_once __DIR__ . "/../utils/basic.inc";

$manager = new MongoDB\Driver\Manager(URI);

$writeConcerns = [0, 1, 2, MongoDB\Driver\WriteConcern::MAJORITY];

foreach ($writeConcerns as $wc) {
    $bulk = new MongoDB\Driver\BulkWrite();
    $bulk->insert(['wc' => $wc]);

    $options = [
        'writeConcern' => new MongoDB\Driver\WriteConcern($wc),
    ];

    $result = $manager->executeBulkWrite(NS, $bulk, $options);
    var_dump($result->isAcknowledged());
    var_dump($result->getInsertedCount());
}

?>
===DONE===
<?php exit(0); ?>
--EXPECT--
bool(false)
NULL
bool(true)
int(1)
bool(true)
int(1)
bool(true)
int(1)
===DONE===
