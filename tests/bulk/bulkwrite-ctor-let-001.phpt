--TEST--
MongoDB\Driver\BulkWrite::__construct(): let option
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

$bulk = new MongoDB\Driver\BulkWrite(['let' => ['id' => 1, 'x' => 'foo']]);
$bulk->insert(['_id' => 1]);
$bulk->insert(['_id' => 2]);
$bulk->delete(['$expr' => ['$eq' => ['$_id', '$$id']]]);
$bulk->update(['_id' => 2], [['$set' => ['x' => '$$x']]]);

$manager->addSubscriber(new CommandLogger);
$manager->executeBulkWrite(NS, $bulk);

$cursor = $manager->executeQuery(NS, new MongoDB\Driver\Query([]));
var_dump($cursor->toArray());

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
insert does not include let option
delete included let: {"id":1,"x":"foo"}
update included let: {"id":1,"x":"foo"}
find does not include let option
array(1) {
  [0]=>
  object(stdClass)#%d (%d) {
    ["_id"]=>
    int(2)
    ["x"]=>
    string(3) "foo"
  }
}
===DONE===
