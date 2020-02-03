--TEST--
PHPC-1274: Implicit sessions are not reused from parent process
--SKIPIF--
<?php if (!function_exists('pcntl_fork')) { die('skip pcntl_fork() not available'); } ?>
<?php require __DIR__ . "/../utils/basic-skipif.inc"; ?>
<?php skip_if_not_libmongoc_crypto(); ?>
<?php skip_if_not_replica_set(); ?>
<?php skip_if_server_version('<', '3.6'); ?>
<?php skip_if_not_clean(); ?>
<?php skip_if_not_clean(DATABASE_NAME, COLLECTION_NAME . '_sessions'); ?>
--FILE--
<?php
require_once __DIR__ . "/../utils/basic.inc";

class SessionLogger implements MongoDB\Driver\Monitoring\CommandSubscriber
{
    private $lsids = [];
    private $logNamespace;
    private $manager;
    private $pid;

    public function __construct(MongoDB\Driver\Manager $manager, $logNamespace)
    {
        $this->logNamespace = $logNamespace;
        $this->manager = $manager;
        $this->pid = getmypid();
    }

    public function executeAndLogSessions(callable $callable)
    {
        $this->lsids = [];

        MongoDB\Driver\Monitoring\addSubscriber($this);
        call_user_func($callable);
        MongoDB\Driver\Monitoring\removeSubscriber($this);

        if (empty($this->lsids)) {
            return;
        }

        $bulk = new MongoDB\Driver\BulkWrite();

        foreach ($this->lsids as $lsid) {
            $bulk->update(['lsid' => $lsid], ['$inc' => ['count' => 1]], ['upsert' => true]);
        }

        $this->manager->executeBulkWrite($this->logNamespace, $bulk);
    }

    public function commandStarted(MongoDB\Driver\Monitoring\CommandStartedEvent $event)
    {
        $command = $event->getCommand();

        if (isset($command->lsid)) {
            $this->lsids[] = $command->lsid;
        }

        $commandName = $event->getCommandName();
        $process = $this->pid === getmypid() ? 'Parent' : 'Child';

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
$logNamespace = NS . '_sessions';
$sessionLogger = new SessionLogger($manager, $logNamespace);

/* This test uses executeBulkWrite() as it's the only execute method that does
 * not create a cursor. The original patch for PHPC-1274 covered those methods
 * that return a cursor but omitted executeBulkWrite(). */
$sessionLogger->executeAndLogSessions(function() use ($manager) {
    $bulk = new MongoDB\Driver\BulkWrite();
    $bulk->insert(['x' => 1]);
    $manager->executeBulkWrite(NS, $bulk);
});

$childPid = pcntl_fork();

if ($childPid === 0) {
    $sessionLogger->executeAndLogSessions(function() use ($manager) {
        $bulk = new MongoDB\Driver\BulkWrite();
        $bulk->insert(['x' => 2]);
        $manager->executeBulkWrite(NS, $bulk);
    });

    echo "Child exits\n";
    exit;
}

if ($childPid > 0) {
    $waitPid = pcntl_waitpid($childPid, $status);

    if ($waitPid === $childPid) {
        echo "Parent waited for child to exit\n";
    }

    $query = new MongoDB\Driver\Query([]);
    $cursor = $manager->executeQuery($logNamespace, $query);

    printf("Sessions used: %d\n", iterator_count($cursor));
}

?>
===DONE===
<?php exit(0); ?>
--EXPECT--
Parent executes insert
Child executes insert
Child exits
Parent waited for child to exit
Sessions used: 2
===DONE===
