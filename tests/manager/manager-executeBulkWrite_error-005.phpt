--TEST--
MongoDB\Driver\Manager::executeBulkWrite() WriteResult accessible for network error
--SKIPIF--
<?php require __DIR__ . "/../utils/basic-skipif.inc"; ?>
<?php skip_if_not_live(); ?>
<?php skip_if_test_commands_disabled(); ?>
<?php skip_if_not_clean(); ?>
--FILE--
<?php
require_once __DIR__ . "/../utils/basic.inc";

$manager = create_test_manager();

// Select a specific server for future operations to avoid mongos switching in sharded clusters
$server = $manager->selectServer(new \MongoDB\Driver\ReadPreference('primary'));

configureTargetedFailPoint($server, 'failCommand', [ 'times' => 1 ], [
  'failCommands' => ['delete'],
  'closeConnection' => true,
]);

$bulk = new MongoDB\Driver\BulkWrite();
$bulk->insert(['x' => 1]);
$bulk->update(['x' => 1], ['$set' => ['y' => 1]]);
$bulk->delete(['x' => 1]);

try {
    $server->executeBulkWrite(NS, $bulk);
} catch (MongoDB\Driver\Exception\BulkWriteException $e) {
    printf("%s(%d): %s\n", get_class($e), $e->getCode(), $e->getMessage());
    $prev = $e->getPrevious();
    printf("%s(%d): %s\n", get_class($prev), $prev->getCode(), $prev->getMessage());
    var_dump($e->getWriteResult());
}

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
MongoDB\Driver\Exception\BulkWriteException(0): Bulk write failed due to previous MongoDB\Driver\Exception\ConnectionTimeoutException: Failed to send "delete" command with database "%s": Failed to read 4 bytes: socket error or timeout
MongoDB\Driver\Exception\ConnectionTimeoutException(%d): Failed to send "delete" command with database "%s": Failed to read 4 bytes: socket error or timeout
object(MongoDB\Driver\WriteResult)#%d (%d) {
  ["insertedCount"]=>
  int(1)
  ["matchedCount"]=>
  int(1)
  ["modifiedCount"]=>
  int(1)
  ["deletedCount"]=>
  int(0)
  ["upsertedCount"]=>
  int(0)
  ["server"]=>
  object(MongoDB\Driver\Server)#%d (%d) {%A
  }
  ["upsertedIds"]=>
  array(0) {
  }
  ["writeErrors"]=>
  array(0) {
  }
  ["writeConcernError"]=>
  NULL
  ["writeConcern"]=>
  object(MongoDB\Driver\WriteConcern)#%d (3) {
    ["w"]=>
    NULL
    ["j"]=>
    NULL
    ["wtimeout"]=>
    int(0)
  }
  ["errorReplies"]=>
  array(0) {
  }
}
===DONE===
