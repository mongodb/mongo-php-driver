--TEST--
MongoDB\Driver\Manager::addSubscriber() with one Manager
--SKIPIF--
<?php require __DIR__ . '/../utils/basic-skipif.inc'; ?>
<?php skip_if_not_live(); ?>
<?php skip_if_not_clean(); ?>
--FILE--
<?php

require_once __DIR__ . '/../utils/basic.inc';

class MySubscriber implements MongoDB\Driver\Monitoring\CommandSubscriber
{
    public function commandStarted(MongoDB\Driver\Monitoring\CommandStartedEvent $event): void
    {
        printf("commandStarted: %s\n", $event->getCommandName());
    }

    public function commandSucceeded(MongoDB\Driver\Monitoring\CommandSucceededEvent $event): void
    {
        printf("commandSucceeded: %s\n", $event->getCommandName());
    }

    public function commandFailed(MongoDB\Driver\Monitoring\CommandFailedEvent $event): void
    {
        printf("commandFailed: %s\n", $event->getCommandName());
    }
}

$m = create_test_manager();

$pingCommand = new MongoDB\Driver\Command(['ping' => 1]);
$unsupportedCommand = new MongoDB\Driver\Command(['unsupportedCommand' => 1]);

printf("ping: %d\n", $m->executeCommand(DATABASE_NAME, $pingCommand)->toArray()[0]->ok);

$subscriber = new MySubscriber;

echo "adding subscriber\n";
$m->addSubscriber($subscriber);

printf("ping: %d\n", $m->executeCommand(DATABASE_NAME, $pingCommand)->toArray()[0]->ok);

throws(function () use ($m, $unsupportedCommand) {
    $m->executeCommand(DATABASE_NAME, $unsupportedCommand);
}, MongoDB\Driver\Exception\CommandException::class);

echo "removing subscriber\n";
$m->removeSubscriber($subscriber);

printf("ping: %d\n", $m->executeCommand(DATABASE_NAME, $pingCommand)->toArray()[0]->ok);

?>
--EXPECT--
ping: 1
adding subscriber
commandStarted: ping
commandSucceeded: ping
ping: 1
commandStarted: unsupportedCommand
commandFailed: unsupportedCommand
OK: Got MongoDB\Driver\Exception\CommandException
removing subscriber
ping: 1
