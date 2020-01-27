--TEST--
MongoDB\Driver\BulkWrite::update() with hint option
--SKIPIF--
<?php require __DIR__ . "/../utils/basic-skipif.inc"; ?>
<?php skip_if_not_live(); ?>
<?php skip_if_server_version('<', '4.2'); ?>
<?php skip_if_not_clean(); ?>
--FILE--
<?php
require_once __DIR__ . "/../utils/basic.inc";

class CommandLogger implements MongoDB\Driver\Monitoring\CommandSubscriber
{
    public function commandStarted(MongoDB\Driver\Monitoring\CommandStartedEvent $event)
    {
        if ($event->getCommandName() !== 'update') {
            return;
        }

        printf("update included hint: %s\n", json_encode($event->getCommand()->updates[0]->hint));
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
$manager->executeBulkWrite(NS, $bulk);

MongoDB\Driver\Monitoring\addSubscriber(new CommandLogger);

$bulk = new MongoDB\Driver\BulkWrite;
$bulk->update(['_id' => 1], ['$set' => ['x' => 11]], ['hint' => '_id_']);
$manager->executeBulkWrite(NS, $bulk);

$bulk = new MongoDB\Driver\BulkWrite;
$bulk->update(['_id' => 2], ['$set' => ['x' => 22]], ['hint' => ['_id' => 1]]);
$manager->executeBulkWrite(NS, $bulk);

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
update included hint: "_id_"
update included hint: {"_id":1}
===DONE===
