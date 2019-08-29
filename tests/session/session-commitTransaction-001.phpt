--TEST--
MongoDB\Driver\Session::commitTransaction() applies w:majority when retrying
--SKIPIF--
<?php require __DIR__ . "/../utils/basic-skipif.inc"; ?>
<?php skip_if_not_libmongoc_crypto() ?>
<?php skip_if_no_transactions(); ?>
<?php skip_if_not_clean(); ?>
--FILE--
<?php
require_once __DIR__ . "/../utils/basic.inc";

class Test implements MongoDB\Driver\Monitoring\CommandSubscriber
{
    private $manager;

    public function __construct()
    {
        $this->manager = new MongoDB\Driver\Manager(URI);

        $this->manager->executeCommand(
            DATABASE_NAME,
            new MongoDB\Driver\Command(['create' => COLLECTION_NAME]),
            ['writeConcern' => new MongoDB\Driver\WriteConcern(MongoDB\Driver\WriteConcern::MAJORITY)]
        );
    }

    public function run(array $startTransactionOptions)
    {
        $session = $this->manager->startSession();

        $session->startTransaction($startTransactionOptions);

        $bulk = new MongoDB\Driver\BulkWrite;
        $bulk->insert(['x' => 1]);
        $this->manager->executeBulkWrite(NS, $bulk, ['session' => $session]);

        MongoDB\Driver\Monitoring\addSubscriber($this);

        $session->commitTransaction();
        $session->commitTransaction();

        MongoDB\Driver\Monitoring\removeSubscriber($this);
    }

    public function commandStarted(MongoDB\Driver\Monitoring\CommandStartedEvent $event)
    {
        if ($event->getCommandName() !== 'commitTransaction') {
            return;
        }

        printf("commitTransaction included write concern: %s\n", json_encode($event->getCommand()->writeConcern));
    }

    public function commandSucceeded(MongoDB\Driver\Monitoring\CommandSucceededEvent $event)
    {
    }

    public function commandFailed(MongoDB\Driver\Monitoring\CommandFailedEvent $event)
    {
    }
}

$test = new Test;

echo "Applies w:majority and default wtimeout when retrying commitTransaction\n";
$test->run(['writeConcern' => new MongoDB\Driver\WriteConcern(1)]);

echo "\nPreserves other WC options when retrying commitTransaction\n";
$test->run(['writeConcern' => new MongoDB\Driver\WriteConcern(1, 5000)]);

?>
===DONE===
<?php exit(0); ?>
--EXPECT--
Applies w:majority and default wtimeout when retrying commitTransaction
commitTransaction included write concern: {"w":1}
commitTransaction included write concern: {"w":"majority","wtimeout":10000}

Preserves other WC options when retrying commitTransaction
commitTransaction included write concern: {"w":1,"wtimeout":5000}
commitTransaction included write concern: {"w":"majority","wtimeout":5000}
===DONE===
