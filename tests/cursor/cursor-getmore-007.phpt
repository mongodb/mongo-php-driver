--TEST--
PHPC-2660: killCursors is sent after a connection error during getMore
--SKIPIF--
<?php require __DIR__ . "/../utils/basic-skipif.inc"; ?>
<?php skip_if_not_libmongoc_crypto(); ?>
<?php skip_if_test_commands_disabled(); ?>
<?php skip_if_not_live(); ?>
<?php skip_if_load_balanced(); ?>
<?php skip_if_not_clean(); ?>
--FILE--
<?php
require_once __DIR__ . "/../utils/basic.inc";

class KillCursorsSubscriber implements MongoDB\Driver\Monitoring\CommandSubscriber
{
    public int $killCursorsCount = 0;

    public function commandStarted(MongoDB\Driver\Monitoring\CommandStartedEvent $event): void
    {
        if ($event->getCommandName() === 'killCursors') {
            $this->killCursorsCount++;
            echo "killCursors was sent\n";
        }
    }

    public function commandSucceeded(MongoDB\Driver\Monitoring\CommandSucceededEvent $event): void {}

    public function commandFailed(MongoDB\Driver\Monitoring\CommandFailedEvent $event): void {}
}

$manager = create_test_manager();

// Select a specific server to target the failpoint
$server = $manager->selectServer(new MongoDB\Driver\ReadPreference('primary'));

$bulk = new MongoDB\Driver\BulkWrite;
$bulk->insert(['_id' => 1]);
$bulk->insert(['_id' => 2]);
$manager->executeBulkWrite(NS, $bulk);

// Create cursor with batchSize=1 to establish a server-side cursor
$cursor = $server->executeQuery(NS, new MongoDB\Driver\Query([], ['batchSize' => 1]));
$iterator = new IteratorIterator($cursor);

// Configure failpoint to close the connection on getMore
configureTargetedFailPoint(
    $server,
    'failCommand',
    ['times' => 1],
    ['failCommands' => ['getMore'], 'closeConnection' => true]
);

$subscriber = new KillCursorsSubscriber;
MongoDB\Driver\Monitoring\addSubscriber($subscriber);

// Trigger getMore which will fail with a connection error
throws(function() use ($iterator) {
    $iterator->next();
}, MongoDB\Driver\Exception\ConnectionTimeoutException::class);

// Destroy the cursor - libmongoc should send killCursors after reconnecting
unset($iterator, $cursor);

MongoDB\Driver\Monitoring\removeSubscriber($subscriber);

printf("killCursors was sent %d time(s)\n", $subscriber->killCursorsCount);

?>
===DONE===
<?php exit(0); ?>
--EXPECT--
OK: Got MongoDB\Driver\Exception\ConnectionTimeoutException
killCursors was sent
killCursors was sent 1 time(s)
===DONE===
