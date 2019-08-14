--TEST--
Retryable reads: executeQuery is retried once
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

$bulk = new MongoDB\Driver\BulkWrite;
$bulk->insert(['x' => 1]);
$bulk->insert(['x' => 2]);

$manager->executeBulkWrite(NS, $bulk);

configureFailPoint($manager, 'failCommand', ['times' => 1], ['failCommands' => ['find'], 'closeConnection' => true]);

$observer = new Observer;
MongoDB\Driver\Monitoring\addSubscriber($observer);

$cursor = $manager->executeQuery(NS, new \MongoDB\Driver\Query(['x' => 1]));
var_dump(iterator_count($cursor));

MongoDB\Driver\Monitoring\removeSubscriber($observer);

?>
===DONE===
<?php exit(0); ?>
--EXPECT--
Command started: find
Command started: find
int(1)
===DONE===
