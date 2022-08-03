--TEST--
MongoDB\Driver\Manager::removeSubscriber() NOP if subscriber not registered
--SKIPIF--
<?php require __DIR__ . '/../utils/basic-skipif.inc'; ?>
<?php skip_if_not_live(); ?>
<?php skip_if_not_clean(); ?>
--FILE--
<?php

require_once __DIR__ . '/../utils/basic.inc';

class MySubscriber implements MongoDB\Driver\Monitoring\CommandSubscriber
{
    private $id;

    public function __construct(string $id)
    {
        $this->id = $id;
    }

    public function commandStarted(MongoDB\Driver\Monitoring\CommandStartedEvent $event): void
    {
        printf("MySubscriber(%s) commandStarted: %s\n", $this->id, $event->getCommandName());
    }

    public function commandSucceeded(MongoDB\Driver\Monitoring\CommandSucceededEvent $event): void
    {
        printf("MySubscriber(%s) commandSucceeded: %s\n", $this->id, $event->getCommandName());
    }

    public function commandFailed(MongoDB\Driver\Monitoring\CommandFailedEvent $event): void
    {
        printf("MySubscriber(%s) commandFailed: %s\n", $this->id, $event->getCommandName());
    }
}

$m = create_test_manager();

$pingCommand = new MongoDB\Driver\Command(['ping' => 1]);

$s1 = new MySubscriber('s1');
$s2 = new MySubscriber('s2');

$m->addSubscriber($s1);
$m->removeSubscriber($s2);

printf("ping: %d\n", $m->executeCommand(DATABASE_NAME, $pingCommand)->toArray()[0]->ok);

?>
--EXPECT--
MySubscriber(s1) commandStarted: ping
MySubscriber(s1) commandSucceeded: ping
ping: 1
