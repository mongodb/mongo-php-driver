--TEST--
PHPC-1274: Session destruct should not end session from parent process
--SKIPIF--
<?php if (!function_exists('pcntl_fork')) { die('skip pcntl_fork() not available'); } ?>
<?php require __DIR__ . "/../utils/basic-skipif.inc"; ?>
<?php skip_if_not_libmongoc_crypto(); ?>
<?php skip_if_not_replica_set(); ?>
<?php skip_if_server_version('<', '3.6'); ?>
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
$session = $manager->startSession();

$bulk = new MongoDB\Driver\BulkWrite();
$bulk->insert(['x' => 1]);
$bulk->insert(['x' => 2]);
$bulk->insert(['x' => 3]);
$manager->executeBulkWrite(NS, $bulk, ['session' => $session]);

MongoDB\Driver\Monitoring\addSubscriber(new CommandLogger);

$query = new MongoDB\Driver\Query([], ['batchSize' => 2]);
$cursor = $manager->executeQuery(NS, $query, ['session' => $session]);

$childPid = pcntl_fork();

if ($childPid === 0) {
    echo "Child exits\n";
    exit;
}

function isSessionOnServer($manager, $session) {
    /* Note: use $listLocalSessions since sessions are only synced to the config
     * database's system.sessions collection every 30 minutes. Alternatively, we
     * could run the refreshLogicalSessionCacheNow command on the primary. */
    $command = new MongoDB\Driver\Command([
        'aggregate' => 1,
        'pipeline' => [
            ['$listLocalSessions' => new stdClass],
            ['$match' => ['_id.id' => $session->getLogicalSessionId()->id]],
        ],
        'cursor' => new stdClass,
    ]);
    $cursor = $manager->executeReadCommand(DATABASE_NAME, $command);

    return iterator_count($cursor) > 0;
}

if ($childPid > 0) {
    $waitPid = pcntl_waitpid($childPid, $status);

    if ($waitPid === $childPid) {
        echo "Parent waited for child to exit\n";
    }

    printf("Session is on server: %s\n", isSessionOnServer($manager, $session) ? 'yes' : 'no');

    printf("Parent fully iterated cursor for %d documents\n", iterator_count($cursor));
}

?>
===DONE===
<?php exit(0); ?>
--EXPECT--
Parent executes find with batchSize: 2
Child exits
Parent waited for child to exit
Parent executes aggregate
Session is on server: yes
Parent executes getMore with batchSize: 2
Parent fully iterated cursor for 3 documents
===DONE===
