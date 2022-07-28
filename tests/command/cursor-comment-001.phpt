--TEST--
MongoDB\Driver\Command comment applies to getMore
--SKIPIF--
<?php require __DIR__ . "/../utils/basic-skipif.inc"; ?>
<?php skip_if_not_live(); ?>
<?php skip_if_server_version('<', '4.4'); ?>
<?php skip_if_not_clean(); ?>
--FILE--
<?php
require_once __DIR__ . "/../utils/basic.inc";

class Test implements MongoDB\Driver\Monitoring\CommandSubscriber
{
    public function executeCommand()
    {
        MongoDB\Driver\Monitoring\addSubscriber($this);

        $manager = create_test_manager();

        $bulkWrite = new MongoDB\Driver\BulkWrite;

        for ($i = 0; $i < 5; $i++) {
            $bulkWrite->insert(['_id' => $i]);
        }

        $writeResult = $manager->executeBulkWrite(NS, $bulkWrite);
        printf("Inserted: %d\n", $writeResult->getInsertedCount());

        $command = new MongoDB\Driver\Command([
            'aggregate' => COLLECTION_NAME,
            'pipeline' => [['$match' => new stdClass]],
            'comment' => ['foo' => 1],
            'cursor' => ['batchSize' => 2]
        ]);
        $cursor = $manager->executeCommand(DATABASE_NAME, $command);

        $cursor->toArray();

        MongoDB\Driver\Monitoring\removeSubscriber($this);
    }

    public function commandStarted(MongoDB\Driver\Monitoring\CommandStartedEvent $event): void
    {
        $command = $event->getCommand();

        if ($event->getCommandName() === 'aggregate' || $event->getCommandName() === 'getMore') {
            printf("%s command includes comment: %s\n", $event->getCommandName(), json_encode($command->comment));
        }
    }

    public function commandSucceeded(MongoDB\Driver\Monitoring\CommandSucceededEvent $event): void
    {
    }

    public function commandFailed(MongoDB\Driver\Monitoring\CommandFailedEvent $event): void
    {
    }
}

(new Test)->executeCommand();

?>
===DONE===
<?php exit(0); ?>
--EXPECT--
Inserted: 5
aggregate command includes comment: {"foo":1}
getMore command includes comment: {"foo":1}
getMore command includes comment: {"foo":1}
===DONE===
