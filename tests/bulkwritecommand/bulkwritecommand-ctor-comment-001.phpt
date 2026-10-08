--TEST--
MongoDB\Driver\BulkWriteCommand::__construct() comment
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
        $command = $event->getCommand();

        if (!isset($command->comment)) {
            printf("%s does not include comment option\n", $event->getCommandName());

            return;
        }

        printf("%s included comment: %s\n", $event->getCommandName(), json_encode($command->comment));
    }

    public function commandSucceeded(MongoDB\Driver\Monitoring\CommandSucceededEvent $event): void
    {
    }

    public function commandFailed(MongoDB\Driver\Monitoring\CommandFailedEvent $event): void
    {
    }
}

$manager = create_test_manager();

$bulk = new MongoDB\Driver\BulkWriteCommand(['comment' => ['foo' => 1]]);
$bulk->insertOne(NS, ['_id' => 1]);

$manager->addSubscriber(new CommandLogger);
$manager->executeBulkWriteCommand($bulk);

$cursor = $manager->executeQuery(NS, new MongoDB\Driver\Query([]));
var_dump($cursor->toArray());

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
bulkWrite included comment: {"foo":1}
find does not include comment option
array(1) {
  [0]=>
  object(stdClass)#%d (%d) {
    ["_id"]=>
    int(1)
  }
}
===DONE===
