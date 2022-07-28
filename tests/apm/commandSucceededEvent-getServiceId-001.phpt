--TEST--
MongoDB\Driver\Monitoring\CommandSucceededEvent includes serviceId for load balanced topology
--SKIPIF--
<?php require __DIR__ . "/../utils/basic-skipif.inc"; ?>
<?php skip_if_not_load_balanced(); ?>
--FILE--
<?php
require_once __DIR__ . "/../utils/basic.inc";

class MySubscriber implements MongoDB\Driver\Monitoring\CommandSubscriber
{
    private $commandStartedServiceId;

    public function commandStarted( \MongoDB\Driver\Monitoring\CommandStartedEvent $event ): void
    {
        printf("commandStarted: %s\n", $event->getCommandName());

        $this->commandStartedServiceId = $event->getServiceId();
        var_dump($this->commandStartedServiceId);
    }

    public function commandSucceeded( \MongoDB\Driver\Monitoring\CommandSucceededEvent $event ): void
    {
        printf("commandSucceeded: %s\n", $event->getCommandName());
        printf("same serviceId as last commandStarted: %s\n", $event->getServiceId() == $this->commandStartedServiceId ? 'yes' : 'no');
        var_dump($event->getServiceId());
    }

    public function commandFailed( \MongoDB\Driver\Monitoring\CommandFailedEvent $event ): void
    {
    }
}

$manager = create_test_manager();
$manager->addSubscriber(new MySubscriber);

$manager->executeCommand(DATABASE_NAME, new MongoDB\Driver\Command(['ping' => 1]));

?>
--EXPECTF--
commandStarted: ping
object(MongoDB\BSON\ObjectId)#%d (%d) {
  ["oid"]=>
  string(24) "%x"
}
commandSucceeded: ping
same serviceId as last commandStarted: yes
object(MongoDB\BSON\ObjectId)#%d (%d) {
  ["oid"]=>
  string(24) "%x"
}
