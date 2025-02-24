--TEST--
MongoDB\Driver\BulkWrite::update() with sort option
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
        if ($event->getCommandName() !== 'update') {
            return;
        }

        printf("update included sort: %s\n", json_encode($event->getCommand()->updates[0]->sort));
    }

    public function commandSucceeded(MongoDB\Driver\Monitoring\CommandSucceededEvent $event): void
    {
    }

    public function commandFailed(MongoDB\Driver\Monitoring\CommandFailedEvent $event): void
    {
    }
}

$manager = create_test_manager();

$bulk = new MongoDB\Driver\BulkWrite();
$bulk->insert(['_id' => 1]);
$bulk->insert(['_id' => 2]);
$bulk->insert(['_id' => 3]);
$manager->executeBulkWrite(NS, $bulk);

MongoDB\Driver\Monitoring\addSubscriber(new CommandLogger);

$bulk = new MongoDB\Driver\BulkWrite;
$bulk->update(['_id' => ['$gt' => 1]], ['$set' => ['x' => 11]], ['sort' => ['_id' => 1]]);
$manager->executeBulkWrite(NS, $bulk);

$bulk = new MongoDB\Driver\BulkWrite;
$bulk->update(['_id' => ['$gt' => 1]], ['x' => 22], ['sort' => ['_id' => -1]]);
$manager->executeBulkWrite(NS, $bulk);

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
