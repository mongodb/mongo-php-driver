--TEST--
MongoDB\Driver\Server::executeBulkWrite() with write concern (replica set primary)
--SKIPIF--
<?php require __DIR__ . "/../utils/basic-skipif.inc"; ?>
<?php NEEDS("REPLICASET"); CLEANUP(REPLICASET); ?>
--FILE--
<?php
require_once __DIR__ . "/../utils/basic.inc";

$manager = new MongoDB\Driver\Manager(REPLICASET);
$server = $manager->selectServer(new MongoDB\Driver\ReadPreference(MongoDB\Driver\ReadPreference::RP_PRIMARY));

$writeConcerns = array(0, 1, 2, MongoDB\Driver\WriteConcern::MAJORITY);

foreach ($writeConcerns as $wc) {
    $bulk = new MongoDB\Driver\BulkWrite();
    $bulk->insert(array('wc' => $wc));

    $result = $server->executeBulkWrite(NS, $bulk, new MongoDB\Driver\WriteConcern($wc));
    var_dump($result->isAcknowledged());
    var_dump($result->getInsertedCount());
}

?>
===DONE===
<?php exit(0); ?>
--EXPECT--
bool(false)
int(0)
bool(true)
int(1)
bool(true)
int(1)
bool(true)
int(1)
===DONE===
