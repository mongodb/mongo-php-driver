--TEST--
MongoDB\Driver\Server::executeBulkWrite() with write concern (replica set secondary, local DB)
--SKIPIF--
<?php require __DIR__ . "/../utils/basic-skipif.inc"; ?>
<?php NEEDS("REPLICASET"); CLEANUP(REPLICASET); ?>
--FILE--
<?php
require_once __DIR__ . "/../utils/basic.inc";

$manager = new MongoDB\Driver\Manager(REPLICASET);
$server = $manager->selectServer(new MongoDB\Driver\ReadPreference(MongoDB\Driver\ReadPreference::RP_SECONDARY));

/* The server ignores write concerns with w>2 for writes to the local database,
 * so we won't test behavior for w=2 and w=majority. */
$writeConcerns = array(0, 1);

foreach ($writeConcerns as $wc) {
    $bulk = new MongoDB\Driver\BulkWrite();
    $bulk->insert(array('wc' => $wc));

    $result = $server->executeBulkWrite('local.' . COLLECTION_NAME, $bulk, new MongoDB\Driver\WriteConcern($wc));
    var_dump($result->isAcknowledged());
    var_dump($result->getInsertedCount());
}

$command = new MongoDB\Driver\Command(array('drop' => COLLECTION_NAME));
$server->executeCommand('local', $command);

?>
===DONE===
<?php exit(0); ?>
--EXPECT--
bool(false)
int(0)
bool(true)
int(1)
===DONE===
