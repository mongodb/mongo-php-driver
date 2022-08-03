--TEST--
MongoDB\Driver\BulkWrite::__construct(): comment option
--SKIPIF--
<?php require __DIR__ . "/../utils/basic-skipif.inc"; ?>
<?php skip_if_not_live(); ?>
<?php skip_if_server_version('<', '4.4'); ?>
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

$bulk = new MongoDB\Driver\BulkWrite(['comment' => ['foo' => 1]]);
$bulk->insert(['_id' => 1]);
$bulk->insert(['_id' => 2]);
$bulk->delete(['_id' => 1]);
$bulk->update(['_id' => 2], ['$set' => ['x' => 1]]);

$manager->addSubscriber(new CommandLogger);
$manager->executeBulkWrite(NS, $bulk);

$cursor = $manager->executeQuery(NS, new MongoDB\Driver\Query([]));
var_dump($cursor->toArray());

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
insert included comment: {"foo":1}
delete included comment: {"foo":1}
update included comment: {"foo":1}
find does not include comment option
array(1) {
  [0]=>
  object(stdClass)#%d (%d) {
    ["_id"]=>
    int(2)
    ["x"]=>
    int(1)
  }
}
===DONE===
