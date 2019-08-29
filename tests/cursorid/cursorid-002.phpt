--TEST--
MongoDB\Driver\CursorID BSON serialization for killCursors command
--SKIPIF--
<?php require __DIR__ . "/../utils/basic-skipif.inc"; ?>
<?php skip_if_not_live(); ?>
<?php skip_if_not_clean(); ?>
<?php skip_if_server_version('<', '3.2'); ?>
--FILE--
<?php

require_once __DIR__ . "/../utils/basic.inc";

$manager = new MongoDB\Driver\Manager(URI);

// Select a specific server for future operations to avoid mongos switching in sharded clusters
$server = $manager->selectServer(new \MongoDB\Driver\ReadPreference('primary'));

$bulk = new MongoDB\Driver\BulkWrite();
$bulk->insert(['_id' => 1]);
$bulk->insert(['_id' => 2]);
$bulk->insert(['_id' => 3]);
$server->executeBulkWrite(NS, $bulk);

$cursor = $server->executeQuery(NS, new MongoDB\Driver\Query([], ['batchSize' => 2]));
$cursorId = $cursor->getId();

$command = new MongoDB\Driver\Command([
    'killCursors' => COLLECTION_NAME,
    'cursors' => [ $cursorId ],
]);

/* Since the killCursors command result includes cursor IDs as 64-bit integers,
 * unserializing the result document requires a 64-bit platform. */
$result = $server->executeCommand(DATABASE_NAME, $command)->toArray()[0];
printf("Killed %d cursor(s)\n", count($result->cursorsKilled));
printf("Killed expected cursor: %s\n", (string) $cursorId === (string) $result->cursorsKilled[0] ? 'yes' : 'no');

?>
===DONE===
<?php exit(0); ?>
--EXPECT--
Killed 1 cursor(s)
Killed expected cursor: yes
===DONE===
