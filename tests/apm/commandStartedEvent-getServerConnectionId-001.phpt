--TEST--
MongoDB\Driver\Monitoring\CommandStartedEvent includes serverConnectionId for 4.2+ server
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

        if (isset($this->commandStartedServerConnectionId)) {
            printf("same serverConnectionId as last commandStarted: %s\n", $event->getServerConnectionId() == $this->commandStartedServerConnectionId ? 'yes' : 'no');
        }

        $this->commandStartedServerConnectionId = $event->getServerConnectionId();
        var_dump($this->commandStartedServerConnectionId);
    }

    public function commandSucceeded(MongoDB\Driver\Monitoring\CommandSucceededEvent $event): void
    {
    }

    public function commandFailed(MongoDB\Driver\Monitoring\CommandFailedEvent $event): void
    {
    }
}

$manager = create_test_manager();
$manager->addSubscriber(new MySubscriber);

// Select a single server in case the topology includes multiple mongoses
$server = $manager->selectServer();

$server->executeCommand(DATABASE_NAME, new MongoDB\Driver\Command(['ping' => 1]));
$server->executeCommand(DATABASE_NAME, new MongoDB\Driver\Command(['ping' => 1]));

?>
--EXPECTF--
commandStarted: ping
int(%d)
commandStarted: ping
same serverConnectionId as last commandStarted: yes
int(%d)
