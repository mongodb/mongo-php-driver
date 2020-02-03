--TEST--
PHPC-1274: Child process can still iterate cursor from parent process
--SKIPIF--
<?php if (!function_exists('pcntl_fork')) { die('skip pcntl_fork() not available'); } ?>
<?php require __DIR__ . "/../utils/basic-skipif.inc"; ?>
<?php skip_if_ssl(); ?>
<?php skip_if_not_live(); ?>
<?php skip_if_server_version('<', '3.2'); ?>
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

$subscriber = new CommandLogger;
MongoDB\Driver\Monitoring\addSubscriber($subscriber);

$query = new MongoDB\Driver\Query([], ['batchSize' => 2]);
$cursor = $manager->executeQuery(NS, $query);

$childPid = pcntl_fork();

if ($childPid === 0) {
    /* This test asserts that mongoc_client_reset() does not prevent child
     * processes from actively interacting with a parent's cursor. Resetting the
     * client only prevents a child from inadvertently closing a parent cursor
     * via its php_phongo_cursor_free_object handler.
     *
     * Note: this test excludes SSL environments because we cannot interact with
     * the socket in a child without affecting the parent (and vice versa) due
     * to the SSL session. */
    printf("Child fully iterated cursor for %d documents\n", iterator_count($cursor));
    echo "Child exits\n";
    exit;
}

if ($childPid > 0) {
    $waitPid = pcntl_waitpid($childPid, $status);

    if ($waitPid === $childPid) {
        echo "Parent waited for child to exit\n";
    }

    echo throws(function() use ($cursor) {
        printf("Parent fully iterated cursor for %d documents\n", iterator_count($cursor));
    }, 'MongoDB\Driver\Exception\RuntimeException'), "\n";
}

/* This test doesn't need to assert that libmongoc may call killCursors when
 * destroying a cursor that previously encountered a CursorNotFound error.
 * See: https://jira.mongodb.org/browse/CDRIVER-3474 */
MongoDB\Driver\Monitoring\removeSubscriber($subscriber);

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
Parent executes find with batchSize: 2
Child executes getMore with batchSize: 2
Child fully iterated cursor for 3 documents
Child exits
Parent waited for child to exit
Parent executes getMore with batchSize: 2
OK: Got MongoDB\Driver\Exception\RuntimeException
%r(Cursor not found, cursor id: \d+|cursor id \d+ not found|Cursor not found \(namespace: '.*', id: \d+\)\.)%r
===DONE===
