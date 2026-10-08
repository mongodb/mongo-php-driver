--TEST--
MongoDB\Driver\BulkWriteCommand::replaceOne() with sort option
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

        printf("update included sort: %s\n", json_encode($event->getCommand()->ops[0]->sort));
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
$bulk->insertOne(NS, ['_id' => 1]);
$bulk->insertOne(NS, ['_id' => 2]);
$bulk->insertOne(NS, ['_id' => 3]);
$manager->executeBulkWriteCommand($bulk);

MongoDB\Driver\Monitoring\addSubscriber(new CommandLogger);

$bulk = new MongoDB\Driver\BulkWriteCommand;
$bulk->replaceOne(NS, ['_id' => ['$gt' => 1]], ['x' => 11], ['sort' => ['_id' => 1]]);
$manager->executeBulkWriteCommand($bulk);

$bulk = new MongoDB\Driver\BulkWriteCommand;
$bulk->replaceOne(NS, ['_id' => ['$gt' => 1]], ['x' => 22], ['sort' => ['_id' => -1]]);
$manager->executeBulkWriteCommand($bulk);

$cursor = $manager->executeQuery(NS, new MongoDB\Driver\Query([]));

var_dump($cursor->toArray());

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
update included sort: {"_id":1}
update included sort: {"_id":-1}
array(3) {
  [0]=>
  object(stdClass)#%d (%d) {
    ["_id"]=>
    int(1)
  }
  [1]=>
  object(stdClass)#%d (%d) {
    ["_id"]=>
    int(2)
    ["x"]=>
    int(11)
  }
  [2]=>
  object(stdClass)#%d (%d) {
    ["_id"]=>
    int(3)
    ["x"]=>
    int(22)
  }
}
===DONE===
