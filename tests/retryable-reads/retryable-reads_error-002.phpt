--TEST--
Retryable reads: executeQuery is not retried when retryable reads are disabled
--SKIPIF--
<?php require __DIR__ . "/../utils/basic-skipif.inc"; ?>
<?php skip_if_not_libmongoc_crypto(); ?>
<?php skip_if_no_failcommand_failpoint(); ?>
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

$manager = new MongoDB\Driver\Manager(URI, ['retryReads' => false]);

// Select a specific server for future operations to avoid mongos switching in sharded clusters
$server = $manager->selectServer(new \MongoDB\Driver\ReadPreference('primary'));

configureTargetedFailPoint($server, 'failCommand', ['times' => 1], ['failCommands' => ['find'], 'closeConnection' => true]);

$observer = new Observer;
MongoDB\Driver\Monitoring\addSubscriber($observer);

throws(
    function() use ($server) {
        $server->executeQuery(NS, new \MongoDB\Driver\Query(['x' => 1]));
    },
    \MongoDB\Driver\Exception\ConnectionTimeoutException::class
);

?>
===DONE===
<?php exit(0); ?>
--EXPECT--
Command started: find
OK: Got MongoDB\Driver\Exception\ConnectionTimeoutException
===DONE===
