--TEST--
MongoDB\Driver\BulkWriteCommand::__construct() multiple options combined
--SKIPIF--
<?php require __DIR__ . "/../utils/basic-skipif.inc"; ?>
<?php skip_if_not_live(); ?>
<?php skip_if_server_version('<', '8.0'); ?>
<?php skip_if_not_clean(); ?>
--FILE--
<?php

require_once __DIR__ . "/../utils/basic.inc";

class CommandLogger implements MongoDB\Driver\Monitoring\CommandSubscriber
{
    public function commandStarted(MongoDB\Driver\Monitoring\CommandStartedEvent $event): void
    {
        if ($event->getCommandName() !== 'bulkWrite') {
            return;
        }

        $command = $event->getCommand();

        printf("ordered: %s\n", var_export($command->ordered, true));
        printf("comment: %s\n", json_encode($command->comment));
    }

    public function commandSucceeded(MongoDB\Driver\Monitoring\CommandSucceededEvent $event): void
    {
    }

    public function commandFailed(MongoDB\Driver\Monitoring\CommandFailedEvent $event): void
    {
    }
}

$manager = create_test_manager();

$bulk = new MongoDB\Driver\BulkWriteCommand([
    'ordered' => false,
    'comment' => 'test comment',
    'verboseResults' => true,
]);
$bulk->insertOne(NS, ['_id' => 1]);
$bulk->insertOne(NS, ['_id' => 2]);

$manager->addSubscriber(new CommandLogger);
$result = $manager->executeBulkWriteCommand($bulk);

var_dump($result->getInsertedCount());
var_dump($result->getInsertResults() !== null);

?>
===DONE===
<?php exit(0); ?>
--EXPECT--
ordered: false
comment: "test comment"
int(2)
bool(true)
===DONE===
