--TEST--
MongoDB\Driver\Server::executeBulkWrite() with write concern (replica set secondary, local DB)
--SKIPIF--
<?php require __DIR__ . "/../utils/basic-skipif.inc"; ?>
<?php skip_if_server_version('>=', '6.2'); /* TODO: Remove this when addressing PHPC-2121 */ ?>
<?php skip_if_not_replica_set(); ?>
<?php skip_if_no_secondary(); ?>
<?php skip_if_not_clean('local', COLLECTION_NAME); ?>
--FILE--
<?php
require_once __DIR__ . "/../utils/basic.inc";

// Disable retryWrites since the test writes to the unreplicated "local" database
// Explicitly use w:1 to work around MongoDB 5.0 applying w:majority (SERVER-61790)
$manager = create_test_manager(URI, ['retryWrites' => false, 'w' => 1]);
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

$bulk = new MongoDB\Driver\BulkWrite();
$bulk->delete( (object) [] );
$server->executeBulkWrite('local.' . COLLECTION_NAME, $bulk);

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
bool(false)

Deprecated: MongoDB\Driver\WriteResult::getInsertedCount(): Calling MongoDB\Driver\WriteResult::getInsertedCount() for an unacknowledged write is deprecated and will throw an exception in ext-mongodb 2.0 in %s
NULL
bool(true)
int(1)
===DONE===
