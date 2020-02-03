--TEST--
PHPC-1274: Child process should not reset parent client multiple times
--SKIPIF--
<?php if (!function_exists('pcntl_fork')) { die('skip pcntl_fork() not available'); } ?>
<?php require __DIR__ . "/../utils/basic-skipif.inc"; ?>
<?php skip_if_not_live(); ?>
<?php skip_if_not_clean(); ?>
--FILE--
<?php
require_once __DIR__ . "/../utils/basic.inc";

class CommandLogger implements MongoDB\Driver\Monitoring\CommandSubscriber
{
    private $pid;

    public function __construct()
    {
        $this->pid = getmypid();
    }

    public function commandStarted(MongoDB\Driver\Monitoring\CommandStartedEvent $event)
    {
        $command = $event->getCommand();
        $commandName = $event->getCommandName();
        $process = $this->pid === getmypid() ? 'Parent' : 'Child';

        if ($commandName === 'find' || $commandName === 'getMore') {
            printf("%s executes %s with batchSize: %d\n", $process, $commandName, $command->batchSize);
            return;
        }

        printf("%s executes %s\n", $process, $commandName);
    }

    public function commandSucceeded(MongoDB\Driver\Monitoring\CommandSucceededEvent $event)
    {
    }

    public function commandFailed(MongoDB\Driver\Monitoring\CommandFailedEvent $event)
    {
    }
}

$manager = new MongoDB\Driver\Manager(URI);

$bulk = new MongoDB\Driver\BulkWrite();
$bulk->insert(['x' => 1]);
$bulk->insert(['x' => 2]);
$bulk->insert(['x' => 3]);
$manager->executeBulkWrite(NS, $bulk);

MongoDB\Driver\Monitoring\addSubscriber(new CommandLogger);

$query = new MongoDB\Driver\Query([], ['batchSize' => 2]);
$cursor = $manager->executeQuery(NS, $query);

$childPid = pcntl_fork();

if ($childPid === 0) {
    /* By having the child execute a query with the parent's client, unsetting
     * the parent's cursor, and then iterating the child's cursor, we test that
     * the parent's client is reset only once and prior to execution of the
     * child's find and getMore commands. If the client were reset multiple
     * times (i.e. between find and getMore), mongoc_cursor_next() would report
     * an error for advancing the cursor after resetting its client. */
    $childCursor = $manager->executeQuery(NS, $query);

    // Unsetting the parent's cursor should not reset its client a second time
    unset($cursor);

    printf("Child fully iterated cursor for %d documents\n", iterator_count($childCursor));
    echo "Child exits\n";
    exit;
}

if ($childPid > 0) {
    $waitPid = pcntl_waitpid($childPid, $status);

    if ($waitPid === $childPid) {
        echo "Parent waited for child to exit\n";
    }

    unset($cursor);
}

?>
===DONE===
<?php exit(0); ?>
--EXPECT--
Parent executes find with batchSize: 2
Child executes find with batchSize: 2
Child executes getMore with batchSize: 2
Child fully iterated cursor for 3 documents
Child exits
Parent waited for child to exit
Parent executes killCursors
===DONE===
