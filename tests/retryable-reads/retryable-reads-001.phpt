--TEST--
Retryable reads: executeReadCommand is retried once
--SKIPIF--
<?php require __DIR__ . "/../utils/basic-skipif.inc"; ?>
<?php skip_if_not_libmongoc_crypto(); ?>
<?php skip_if_no_failcommand_failpoint(); ?>
<?php skip_if_not_clean(); ?>
--FILE--
<?php
require_once __DIR__ . "/../utils/basic.inc";

class Observer implements MongoDB\Driver\Monitoring\CommandSubscriber
{
    public function commandStarted(MongoDB\Driver\Monitoring\CommandStartedEvent $event)
    {
        printf("Command started: %s\n", $event->getCommandName());
    }

    public function commandSucceeded(MongoDB\Driver\Monitoring\CommandSucceededEvent $event)
    {
    }

    public function commandFailed(MongoDB\Driver\Monitoring\CommandFailedEvent $event)
    {
    }
}

$manager = new MongoDB\Driver\Manager(URI, ['retryReads' => true]);

// Select a specific server for future operations to avoid mongos switching in sharded clusters
$server = $manager->selectServer(new \MongoDB\Driver\ReadPreference('primary'));

$bulk = new MongoDB\Driver\BulkWrite;
$bulk->insert(['x' => 1]);
$bulk->insert(['x' => 2]);

$server->executeBulkWrite(NS, $bulk);

configureTargetedFailPoint($server, 'failCommand', ['times' => 1], ['failCommands' => ['aggregate'], 'closeConnection' => true]);

$observer = new Observer;
MongoDB\Driver\Monitoring\addSubscriber($observer);

$command = new MongoDB\Driver\Command([
    'aggregate' => COLLECTION_NAME,
    'pipeline' => [
        ['$group' => ['_id' => 1, 'n' => ['$sum' => 1]]],
    ],
    'cursor' => (object) [],
]);
$cursor = $server->executeReadCommand(DATABASE_NAME, $command);
var_dump(iterator_to_array($cursor));

MongoDB\Driver\Monitoring\removeSubscriber($observer);

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
Command started: aggregate
Command started: aggregate
array(1) {
  [0]=>
  object(stdClass)#%d (2) {
    ["_id"]=>
    int(1)
    ["n"]=>
    int(2)
  }
}
===DONE===
