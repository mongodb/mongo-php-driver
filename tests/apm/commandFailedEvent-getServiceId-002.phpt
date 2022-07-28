--TEST--
MongoDB\Driver\Monitoring\CommandFailedEvent omits serviceId for non-load balanced topology
--SKIPIF--
<?php require __DIR__ . "/../utils/basic-skipif.inc"; ?>
<?php skip_if_load_balanced(); ?>
--FILE--
<?php
require_once __DIR__ . "/../utils/basic.inc";

class MySubscriber implements MongoDB\Driver\Monitoring\CommandSubscriber
{
    public function commandStarted(MongoDB\Driver\Monitoring\CommandStartedEvent $event): void
    {
        printf("commandStarted: %s\n", $event->getCommandName());
        var_dump($event->getServiceId());
    }

    public function commandSucceeded(MongoDB\Driver\Monitoring\CommandSucceededEvent $event): void
    {
    }

    public function commandFailed( \MongoDB\Driver\Monitoring\CommandFailedEvent $event ): void
    {
        printf("commandFailed: %s\n", $event->getCommandName());
        var_dump($event->getServiceId());
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
NULL
commandFailed: aggregate
NULL
OK: Got MongoDB\Driver\Exception\CommandException
