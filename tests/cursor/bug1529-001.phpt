--TEST--
PHPC-1529: Resetting a client should also reset the keyVaultClient
--SKIPIF--
<?php if (!function_exists('pcntl_fork')) { die('skip pcntl_fork() not available'); } ?>
<?php require __DIR__ . "/../utils/basic-skipif.inc"; ?>
<?php skip_if_not_live(); ?>
<?php skip_if_server_version('<', '4.2'); ?>
<?php skip_if_not_libmongocrypt(); ?>
<?php skip_if_not_clean(); ?>
<?php skip_if_crypt_shared(); // Build fails due to SERVER-71049 ?>
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

    public function commandStarted(MongoDB\Driver\Monitoring\CommandStartedEvent $event): void
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

    public function commandSucceeded(MongoDB\Driver\Monitoring\CommandSucceededEvent $event): void
    {
    }

    public function commandFailed(MongoDB\Driver\Monitoring\CommandFailedEvent $event): void
    {
    }
}

$keyVaultClient = create_test_manager(URI, [], ['disableClientPersistence' => true]);
$autoEncryptionOpts = [
    'keyVaultClient' => $keyVaultClient,
    'keyVaultNamespace' => CSFLE_KEY_VAULT_NS,
    'kmsProviders' => ['local' => ['key' => new MongoDB\BSON\Binary(CSFLE_LOCAL_KEY, 0)]],
];

$manager = create_test_manager(URI, [], ['autoEncryption' => $autoEncryptionOpts, 'disableClientPersistence' => true]);

$bulk = new MongoDB\Driver\BulkWrite();
$bulk->insert(['x' => 1]);
$bulk->insert(['x' => 2]);
$bulk->insert(['x' => 3]);
$keyVaultClient->executeBulkWrite(NS, $bulk);

MongoDB\Driver\Monitoring\addSubscriber(new CommandLogger);

$query = new MongoDB\Driver\Query([], ['batchSize' => 2]);
$cursor = $keyVaultClient->executeQuery(NS, $query);

$childPid = pcntl_fork();

if ($childPid === 0) {
    /* Executing any operation with the parent's client resets this client as well as
     * the keyVaultClient. Continuing iteration of the cursor opened on the
     * keyVaultClient before resetting it should then result in an error due to
     * the client having been reset. */
    $manager->executeCommand(DATABASE_NAME, new MongoDB\Driver\Command(['ping' => 1]));

    echo throws(
        function () use ($cursor) { iterator_count($cursor); },
        MongoDB\Driver\Exception\RuntimeException::class
    ), "\n";

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
Child executes ping
OK: Got MongoDB\Driver\Exception\RuntimeException
Cannot advance cursor after client reset
Child exits
Parent waited for child to exit
Parent executes killCursors
===DONE===
