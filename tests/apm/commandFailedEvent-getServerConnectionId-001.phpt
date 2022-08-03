--TEST--
MongoDB\Driver\Monitoring\CommandFailedEvent includes serverConnectionId for 4.2+ server
--SKIPIF--
<?php require __DIR__ . "/../utils/basic-skipif.inc"; ?>
<?php skip_if_not_live(); ?>
<?php skip_if_server_version('<', '4.2'); ?>
--FILE--
<?php
require_once __DIR__ . "/../utils/basic.inc";

class MySubscriber implements MongoDB\Driver\Monitoring\CommandSubscriber
{
    private $commandStartedServerConnectionId;

    public function commandStarted(MongoDB\Driver\Monitoring\CommandStartedEvent $event): void
    {
        printf("commandStarted: %s\n", $event->getCommandName());

        $this->commandStartedServerConnectionId = $event->getServerConnectionId();
        var_dump($this->commandStartedServerConnectionId);
    }

    public function commandSucceeded(MongoDB\Driver\Monitoring\CommandSucceededEvent $event): void
    {
    }

    public function commandFailed( \MongoDB\Driver\Monitoring\CommandFailedEvent $event ): void
    {
        printf("commandFailed: %s\n", $event->getCommandName());
        printf("same serverConnectionId as last commandStarted: %s\n", $event->getServerConnectionId() == $this->commandStartedServerConnectionId ? 'yes' : 'no');
        var_dump($event->getServerConnectionId());
    }
}

$manager = create_test_manager();
$manager->addSubscriber(new MySubscriber);

$command = new MongoDB\Driver\Command([
    'aggregate' => COLLECTION_NAME,
    'pipeline' => [['$unsupported' => 1]],
]);

throws(function() use ($manager, $command) {
    $manager->executeCommand(DATABASE_NAME, $command);
}, MongoDB\Driver\Exception\CommandException::class);

?>
--EXPECTF--
commandStarted: aggregate
int(%d)
commandFailed: aggregate
same serverConnectionId as last commandStarted: yes
int(%d)
OK: Got MongoDB\Driver\Exception\CommandException
