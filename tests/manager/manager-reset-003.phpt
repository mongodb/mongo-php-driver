--TEST--
MongoDB\Driver\reset: Resetting a manager causes socket errors on SSL connections
--SKIPIF--
<?php if (!function_exists('pcntl_fork')) { die('skip pcntl_fork() not available'); } ?>
<?php require __DIR__ . "/../utils/basic-skipif.inc"; ?>
<?php skip_if_not_live(); ?>
<?php skip_if_not_clean(); ?>
<?php skip_if_not_ssl(); ?>
--FILE--
<?php
require_once __DIR__ . "/../utils/basic.inc";

class CommandLogger implements MongoDB\Driver\Monitoring\CommandSubscriber
{
    public function commandStarted(MongoDB\Driver\Monitoring\CommandStartedEvent $event)
    {
        $command = $event->getCommand();

        if ($event->getCommandName() === 'find') {
            printf("find command specifies batchSize: %d\n", $command->batchSize);
        }

        if ($event->getCommandName() === 'getMore') {
            printf("getMore command specifies batchSize: %d\n", $command->batchSize);
        }
    }

    public function commandSucceeded(MongoDB\Driver\Monitoring\CommandSucceededEvent $event)
    {
    }

    public function commandFailed(MongoDB\Driver\Monitoring\CommandFailedEvent $event)
    {
    }
}

MongoDB\Driver\Monitoring\addSubscriber(new CommandLogger);

$manager = new MongoDB\Driver\Manager(URI);

$bulk = new MongoDB\Driver\BulkWrite();
$bulk->insert(['x' => 1]);
$bulk->insert(['x' => 2]);
$bulk->insert(['x' => 3]);
$manager->executeBulkWrite(NS, $bulk);

$query = new MongoDB\Driver\Query([], ['batchSize' => 2]);
$cursor = $manager->executeQuery(NS, $query);

$parentPid = getmypid();
$childPid = pcntl_fork();

if ($childPid === 0) {
    MongoDB\Driver\reset($manager);

    echo "Child exits\n";
    exit;
}

if ($childPid > 0) {
    $waitPid = pcntl_waitpid($childPid, $status);

    if ($waitPid === $childPid) {
        echo "Parent waited for child to exit\n";
    }

    echo throws(function () use ($cursor) {
        iterator_count($cursor);
    }, MongoDB\Driver\Exception\ConnectionTimeoutException::class), "\n";
}

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
find command specifies batchSize: 2
Child exits
Parent waited for child to exit
getMore command specifies batchSize: 2
OK: Got MongoDB\Driver\Exception\ConnectionTimeoutException
Failed to send "getMore" command with database "%s": Failed to read 4 bytes: socket error or timeout
===DONE===
