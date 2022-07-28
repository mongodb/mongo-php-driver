--TEST--
MongoDB\Driver\Manager::addSubscriber() with multiple Managers
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

/* The first two Managers will share the same libmongoc client. The third will
 * use a different client. */
$m1 = create_test_manager();
$m2 = create_test_manager();
$m3 = create_test_manager(null, [], ['disableClientPersistence' => true]);

$s1 = new MySubscriber('s1_on_m1');
$s2 = new MySubscriber('s2_on_m3');

$m1->addSubscriber($s1);
$m3->addSubscriber($s2);

$pingCommand = new MongoDB\Driver\Command(['ping' => 1]);
$unsupportedCommand = new MongoDB\Driver\Command(['unsupportedCommand' => 1]);

printf("ping: %d\n", $m1->executeCommand(DATABASE_NAME, $pingCommand)->toArray()[0]->ok);
// s1_on_m1 will be notified because both Managers share the same client
printf("ping: %d\n", $m2->executeCommand(DATABASE_NAME, $pingCommand)->toArray()[0]->ok);
printf("ping: %d\n", $m3->executeCommand(DATABASE_NAME, $pingCommand)->toArray()[0]->ok);

throws(function () use ($m1, $unsupportedCommand) {
    $m1->executeCommand(DATABASE_NAME, $unsupportedCommand);
}, MongoDB\Driver\Exception\CommandException::class);

throws(function () use ($m2, $unsupportedCommand) {
    $m2->executeCommand(DATABASE_NAME, $unsupportedCommand);
}, MongoDB\Driver\Exception\CommandException::class);

throws(function () use ($m3, $unsupportedCommand) {
    $m3->executeCommand(DATABASE_NAME, $unsupportedCommand);
}, MongoDB\Driver\Exception\CommandException::class);

?>
--EXPECT--
MySubscriber(s1_on_m1) commandStarted: ping
MySubscriber(s1_on_m1) commandSucceeded: ping
ping: 1
MySubscriber(s1_on_m1) commandStarted: ping
MySubscriber(s1_on_m1) commandSucceeded: ping
ping: 1
MySubscriber(s2_on_m3) commandStarted: ping
MySubscriber(s2_on_m3) commandSucceeded: ping
ping: 1
MySubscriber(s1_on_m1) commandStarted: unsupportedCommand
MySubscriber(s1_on_m1) commandFailed: unsupportedCommand
OK: Got MongoDB\Driver\Exception\CommandException
MySubscriber(s1_on_m1) commandStarted: unsupportedCommand
MySubscriber(s1_on_m1) commandFailed: unsupportedCommand
OK: Got MongoDB\Driver\Exception\CommandException
MySubscriber(s2_on_m3) commandStarted: unsupportedCommand
MySubscriber(s2_on_m3) commandFailed: unsupportedCommand
OK: Got MongoDB\Driver\Exception\CommandException
