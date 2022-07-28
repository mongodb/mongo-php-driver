--TEST--
MongoDB\Driver\Monitoring\CommandSucceededEvent omits serviceId for non-load balanced topology
--SKIPIF--
<?php require __DIR__ . "/../utils/basic-skipif.inc"; ?>
<?php skip_if_load_balanced(); ?>
--FILE--
<?php
require_once __DIR__ . "/../utils/basic.inc";

class MySubscriber implements MongoDB\Driver\Monitoring\CommandSubscriber
{
    public function commandStarted( \MongoDB\Driver\Monitoring\CommandStartedEvent $event ): void
    {
        printf("commandStarted: %s\n", $event->getCommandName());
        var_dump($event->getServiceId());
    }

    public function commandSucceeded( \MongoDB\Driver\Monitoring\CommandSucceededEvent $event ): void
    {
        printf("commandSucceeded: %s\n", $event->getCommandName());
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
NULL
commandSucceeded: ping
NULL
