--TEST--
PHPC-1050: Command cursor should not invoke getMore at execution
--SKIPIF--
<?php require __DIR__ . "/../utils/basic-skipif.inc"; ?>
<?php skip_if_not_replica_set(); ?>
<?php skip_if_not_server_storage_engine('wiredTiger'); ?>
<?php skip_if_not_clean(); ?>
--FILE--
<?php
require_once __DIR__ . "/../utils/basic.inc";

class CommandLogger implements MongoDB\Driver\Monitoring\CommandSubscriber
{
    public function commandStarted(MongoDB\Driver\Monitoring\CommandStartedEvent $event): void
    {
        if ($event->getCommandName() !== 'aggregate' && $event->getCommandName() !== 'getMore') {
            return;
        }

        printf("Executing command: %s\n", $event->getCommandName());
    }

    public function commandSucceeded(MongoDB\Driver\Monitoring\CommandSucceededEvent $event): void
    {
        if ($event->getCommandName() !== 'aggregate' && $event->getCommandName() !== 'getMore') {
            return;
        }

        printf("Executing command took %0.6f seconds\n", $event->getDurationMicros() / 1000000);
    }

    public function commandFailed(MongoDB\Driver\Monitoring\CommandFailedEvent $event): void
    {
    }
}

$manager = create_test_manager();

$cmd = new MongoDB\Driver\Command(
    [
        'aggregate' => COLLECTION_NAME,
        'pipeline' => [
            ['$changeStream' => (object) []],
        ],
        'cursor' => (object) [],
    ],
    [
        'maxAwaitTimeMS' => 500,
    ]
);

MongoDB\Driver\Monitoring\addSubscriber(new CommandLogger);

$cursor = $manager->executeReadCommand(DATABASE_NAME, $cmd);

$it = new IteratorIterator($cursor);

printf("Current position is valid: %s\n\n", $it->valid() ? 'yes' : 'no');

echo "Rewinding cursor\n";
$it->rewind();

printf("Current position is valid: %s\n\n", $it->valid() ? 'yes' : 'no');

$bulk = new MongoDB\Driver\BulkWrite;
$bulk->insert(['x' => 1]);
$manager->executeBulkWrite(NS, $bulk);

echo "Advancing cursor\n";
$it->next();

printf("Current position is valid: %s\n\n", $it->valid() ? 'yes' : 'no');

$document = $it->current();

if (isset($document)) {
    printf("Operation type: %s\n", $document->operationType);
    var_dump($document->fullDocument);
}

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
Executing command: aggregate
Executing command took 0.%d seconds
Current position is valid: no

Rewinding cursor
Executing command: getMore
Executing command took 0.%r(4|5)%r%d seconds
Current position is valid: no

Advancing cursor
Executing command: getMore
Executing command took 0.%d seconds
Current position is valid: yes

Operation type: insert
object(stdClass)#%d (%d) {
  ["_id"]=>
  object(MongoDB\BSON\ObjectId)#%d (%d) {
    ["oid"]=>
    string(24) "%x"
  }
  ["x"]=>
  int(1)
}
===DONE===
