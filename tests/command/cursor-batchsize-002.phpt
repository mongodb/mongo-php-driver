--TEST--
MongoDB\Driver\Command batchSize of zero is ignored for getMore
--SKIPIF--
<?php require __DIR__ . "/../utils/basic-skipif.inc"; ?>
<?php skip_if_not_live(); ?>
<?php skip_if_not_clean(); ?>
--FILE--
<?php
require_once __DIR__ . "/../utils/basic.inc";

class Test implements MongoDB\Driver\Monitoring\CommandSubscriber
{
    public function executeCommand()
    {
        MongoDB\Driver\Monitoring\addSubscriber($this);

        $manager = new MongoDB\Driver\Manager(URI);

        $bulkWrite = new MongoDB\Driver\BulkWrite;

        for ($i = 0; $i < 5; $i++) {
           $bulkWrite->insert(['_id' => $i]);
        }

        $writeResult = $manager->executeBulkWrite(NS, $bulkWrite);
        printf("Inserted: %d\n", $writeResult->getInsertedCount());

        $command = new MongoDB\Driver\Command([
            'aggregate' => COLLECTION_NAME,
            'pipeline' => [['$match' => new stdClass]],
            'cursor' => ['batchSize' => 0]
        ]);
        $cursor = $manager->executeCommand(DATABASE_NAME, $command);

        $cursor->toArray();

        MongoDB\Driver\Monitoring\removeSubscriber($this);
    }

    public function commandStarted(MongoDB\Driver\Monitoring\CommandStartedEvent $event)
    {
        $command = $event->getCommand();

        if ($event->getCommandName() === 'aggregate') {
            printf("aggregate command specifies batchSize: %d\n", $command->cursor->batchSize);
        }

        if ($event->getCommandName() === 'getMore') {
            printf("getMore command specifies batchSize: %s\n", isset($command->batchSize) ? 'yes' : 'no');
        }
    }

    public function commandSucceeded(MongoDB\Driver\Monitoring\CommandSucceededEvent $event)
    {
        $reply = $event->getReply();

        if ($event->getCommandName() === 'aggregate') {
            printf("aggregate response contains %d document(s)\n", count($reply->cursor->firstBatch));
        }

        if ($event->getCommandName() === 'getMore') {
            printf("getMore response contains %d document(s)\n", count($reply->cursor->nextBatch));
        }
    }

    public function commandFailed(MongoDB\Driver\Monitoring\CommandFailedEvent $event)
    {
    }
}

(new Test)->executeCommand();

?>
===DONE===
<?php exit(0); ?>
--EXPECT--
Inserted: 5
aggregate command specifies batchSize: 0
aggregate response contains 0 document(s)
getMore command specifies batchSize: no
getMore response contains 5 document(s)
===DONE===
