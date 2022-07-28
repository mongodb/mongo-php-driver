--TEST--
MongoDB\Driver\Manager::addSubscriber() adds reference to subscriber
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

    public function __destruct()
    {
        echo __METHOD__, "\n";
    }
}

$m = create_test_manager();

$pingCommand = new MongoDB\Driver\Command(['ping' => 1]);

$subscriber = new MySubscriber;

echo "adding subscriber\n";
$m->addSubscriber($subscriber);

printf("ping: %d\n", $m->executeCommand(DATABASE_NAME, $pingCommand)->toArray()[0]->ok);

echo "unsetting subscriber\n";
unset($subscriber);

printf("ping: %d\n", $m->executeCommand(DATABASE_NAME, $pingCommand)->toArray()[0]->ok);

echo "unsetting manager\n";
unset($manager);

?>
--EXPECT--
adding subscriber
commandStarted: ping
commandSucceeded: ping
ping: 1
unsetting subscriber
commandStarted: ping
commandSucceeded: ping
ping: 1
unsetting manager
MySubscriber::__destruct
