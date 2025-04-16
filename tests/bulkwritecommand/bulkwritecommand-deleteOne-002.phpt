--TEST--
MongoDB\Driver\BulkWriteCommand::deleteOne() with hint option
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

        printf("delete included hint: %s\n", json_encode($event->getCommand()->ops[0]->hint));
    }

    public function commandSucceeded(MongoDB\Driver\Monitoring\CommandSucceededEvent $event): void
    {
    }

    public function commandFailed(MongoDB\Driver\Monitoring\CommandFailedEvent $event): void
    {
    }
}

$manager = create_test_manager();

$bulk = new MongoDB\Driver\BulkWriteCommand();
$bulk->insertOne(NS, ['x' => 1]);
$bulk->insertOne(NS, ['x' => 2]);
$manager->executeBulkWriteCommand($bulk);

MongoDB\Driver\Monitoring\addSubscriber(new CommandLogger);

$bulk = new MongoDB\Driver\BulkWriteCommand;
$bulk->deleteOne(NS, ['_id' => 1], ['hint' => '_id_']);
$manager->executeBulkWriteCommand($bulk);

$bulk = new MongoDB\Driver\BulkWriteCommand;
$bulk->deleteOne(NS, ['_id' => 2], ['hint' => ['_id' => 1]]);
$manager->executeBulkWriteCommand($bulk);

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
delete included hint: "_id_"
delete included hint: {"_id":1}
===DONE===
