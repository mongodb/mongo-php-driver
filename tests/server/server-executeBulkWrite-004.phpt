--TEST--
MongoDB\Driver\Server::executeBulkWrite() with write concern (replica set secondary)
--SKIPIF--
<?php require __DIR__ . "/../utils/basic-skipif.inc"; ?>
<?php NEEDS("REPLICASET"); CLEANUP(REPLICASET); ?>
--FILE--
<?php
require_once __DIR__ . "/../utils/basic.inc";

$manager = new MongoDB\Driver\Manager(REPLICASET);
$server = $manager->selectServer(new MongoDB\Driver\ReadPreference(MongoDB\Driver\ReadPreference::RP_SECONDARY));

$bulk = new MongoDB\Driver\BulkWrite();
$bulk->insert(array('wc' => 0));

$result = $server->executeBulkWrite(NS, $bulk, new MongoDB\Driver\WriteConcern(0));
var_dump($result->isAcknowledged());
var_dump($result->getInsertedCount());

$writeConcerns = array(1, 2, MongoDB\Driver\WriteConcern::MAJORITY);

foreach ($writeConcerns as $wc) {
    $bulk = new MongoDB\Driver\BulkWrite();
    $bulk->insert(array('wc' => $wc));

    echo throws(function() use ($server, $bulk, $wc) {
        $server->executeBulkWrite(NS, $bulk, new MongoDB\Driver\WriteConcern($wc));
    }, "MongoDB\Driver\Exception\RuntimeException"), "\n";
}

?>
===DONE===
<?php exit(0); ?>
--EXPECT--
bool(false)
int(0)
OK: Got MongoDB\Driver\Exception\RuntimeException
not master
OK: Got MongoDB\Driver\Exception\RuntimeException
not master
OK: Got MongoDB\Driver\Exception\RuntimeException
not master
===DONE===
