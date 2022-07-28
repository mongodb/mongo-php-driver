--TEST--
MongoDB\Driver\Query::__construct(): let option
--SKIPIF--
<?php require __DIR__ . "/../utils/basic-skipif.inc"; ?>
<?php skip_if_not_live(); ?>
<?php skip_if_server_version('<', '5.0'); ?>
<?php skip_if_not_clean(); ?>
--FILE--
<?php

require_once __DIR__ . "/../utils/basic.inc";

class CommandLogger implements MongoDB\Driver\Monitoring\CommandSubscriber
{
    public function commandStarted(MongoDB\Driver\Monitoring\CommandStartedEvent $event): void
    {
        $command = $event->getCommand();

        if (!isset($command->let)) {
            printf("%s does not include let option\n", $event->getCommandName());

            return;
        }

        printf("%s included let: %s\n", $event->getCommandName(), json_encode($command->let));
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
    ['$expr' => ['$eq' => ['$_id', '$$id']]],
    ['let' => ['id' => 1]]
);

$manager->addSubscriber(new CommandLogger);
$cursor = $manager->executeQuery(NS, $query);
var_dump($cursor->toArray());

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
find included let: {"id":1}
array(1) {
  [0]=>
  object(stdClass)#%d (%d) {
    ["_id"]=>
    int(1)
  }
}
===DONE===
