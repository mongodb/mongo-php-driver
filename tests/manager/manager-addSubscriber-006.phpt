--TEST--
MongoDB\Driver\Manager::addSubscriber() subscriber is only notified once (two Managers)
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

// Subscribers will share the same libmongoc client
$m1 = create_test_manager();
$m2 = create_test_manager();

$pingCommand = new MongoDB\Driver\Command(['ping' => 1]);

$subscriber = new MySubscriber;

$m1->addSubscriber($subscriber);
$m2->addSubscriber($subscriber);

printf("ping: %d\n", $m1->executeCommand(DATABASE_NAME, $pingCommand)->toArray()[0]->ok);
printf("ping: %d\n", $m2->executeCommand(DATABASE_NAME, $pingCommand)->toArray()[0]->ok);

?>
--EXPECT--
commandStarted: ping
commandSucceeded: ping
ping: 1
commandStarted: ping
commandSucceeded: ping
ping: 1
