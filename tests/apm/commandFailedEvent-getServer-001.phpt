--TEST--
MongoDB\Driver\Monitoring\CommandFailedEvent::getServer()
--SKIPIF--
<?php require __DIR__ . "/../utils/basic-skipif.inc"; ?>
<?php skip_if_not_live(); ?>
<?php skip_if_not_clean(); ?>
--FILE--
<?php
require_once __DIR__ . "/../utils/basic.inc";

class MySubscriber implements MongoDB\Driver\Monitoring\CommandSubscriber
{
    public function commandStarted(MongoDB\Driver\Monitoring\CommandStartedEvent $event): void
    {
    }

    public function commandSucceeded(MongoDB\Driver\Monitoring\CommandSucceededEvent $event): void
    {
    }

    public function commandFailed(MongoDB\Driver\Monitoring\CommandFailedEvent $event): void
    {
        var_dump($event->getServer());
    }
}

$manager = create_test_manager();

$subscriber = new MySubscriber();
MongoDB\Driver\Monitoring\addSubscriber($subscriber);

$command = new MongoDB\Driver\Command(['unsupportedCommand' => 1]);

try {
    $manager->executeCommand('admin', $command);
} catch (Exception $e) {
}

?>
--EXPECTF--

Deprecated: Method MongoDB\Driver\Monitoring\CommandFailedEvent::getServer() is deprecated in %s
object(MongoDB\Driver\Server)#%d (%d) {
  %A
}
