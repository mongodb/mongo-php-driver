--TEST--
MongoDB\Driver\Query::__construct(): comment option
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

$bulk = new MongoDB\Driver\BulkWrite;
$bulk->insert(['_id' => 1]);
$bulk->insert(['_id' => 2]);
$manager->executeBulkWrite(NS, $bulk);

$query = new MongoDB\Driver\Query(
    ['_id' => 1],
    ['comment' => ['foo' => 1]]
);

$manager->addSubscriber(new CommandLogger);
$cursor = $manager->executeQuery(NS, $query);
var_dump($cursor->toArray());

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
find included comment: {"foo":1}
array(1) {
  [0]=>
  object(stdClass)#%d (%d) {
    ["_id"]=>
    int(1)
  }
}
===DONE===
